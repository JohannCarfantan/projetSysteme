#include "divers.h"
#include "commandes_externes.h"
#include <sys/wait.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>

t_bool	ActionEXEC (parse_info *info, int debut, int nbArg) {
  char ligne[CHAINE_MAX];
  t_bool premierPlan;
  int i;


  char *arg[nbArg];

  strcpy(ligne, "");
  for(i=0; i<nbArg; i++) {

    if(strlen(ligne)!=0) {
      strcat(ligne," ");
    }
    strcat(ligne, info->ligne_cmd[debut+i]);
    arg[i]= info->ligne_cmd[debut+i];
    }

    arg[nbArg]=NULL;

    premierPlan = (info->modificateur[debut]!=ARRIERE_PLAN);

    //Premier plan = 0 si Arrière plan
    //Premier plan = 1 si Premier plan

    printf("Premier plan : %d\n", premierPlan);
    //printf("Modificateur[debut] : %d\n", info->modificateur[debut]);

    for (int i=0;i<nbArg;i++)
    {
    printf("arg : %s \n", arg[i]);
    }

    //Recupération fichier de sortie et d'entrée
    char *fileSortie = info->sortie;
    char *fileEntree = info->entree;

    //printf("fileEntree %s \n",fileEntree);
    //printf("fileSortie %s \n",fileSortie);

    //Création d'un processus fils afin d'executer une commande externne
    pid_t pid_fils = fork();
    if ( pid_fils == -1) {
    perror ("Echec du fork 1\n");
    return EXIT_FAILURE;
    }
    if( pid_fils ==0){ //Code qu'execute le processus fils

      if(fileSortie!=NULL) //Si il y a un fichier en sortie '>'
      {
      int fd = open(fileSortie, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); //Ouverture fichier

      dup2(fd, 1);   // make stdout go to file
      dup2(fd, 2);   // make stderr go to file - you may choose to not do this

      close(fd);
      }

      if(fileEntree!=NULL) //Si il y a un fichier en entrée '<'
      {
      int fd = open(fileEntree, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); //Redirection entrée Fichier

      dup2(fd, 0);   // stdin

      close(fd);
      }

      //Execution commande externe avec tableau d'arguments 
      int res = execvp(*(&arg[0]),&arg[0]);
      (void) res; //Code non utilisé 
      //printf("Res CE : %d \n", res);

    }

  //printf("execution d'une commande externe  (%s %d) a ecrire :\n%s\n", __FILE__, __LINE__, ligne);


  if (premierPlan==1) //Attend le processus fils et donc bloque le shell mais si aucune attente rique de processus zombie
  {
    wait(NULL);
  }

  return faux;
}
