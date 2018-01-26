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
    printf("Modificateur[debut] : %d\n", info->modificateur[debut]);

    for (int i=0;i<nbArg;i++)
    {
    printf("arg : %s \n", arg[i]);
    }

    char *file = info->sortie;

    printf("File %s \n",file);

    pid_t pid_fils = fork();
    if ( pid_fils == -1) {
    perror ("Echec du fork 1\n");
    return EXIT_FAILURE;
    }
    if( pid_fils ==0){

      if(file!=NULL)
      {
      int fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

      dup2(fd, 1);   // make stdout go to file
      dup2(fd, 2);   // make stderr go to file - you may choose to not do this

      close(fd);
      }

      int res = execvp(*(&arg[0]),&arg[0]);
      (void) res;
      //printf("Res CE : %d \n", res);

    }

  //printf("execution d'une commande externe  (%s %d) a ecrire :\n%s\n", __FILE__, __LINE__, ligne);


  if (premierPlan==1)
  {
    wait(NULL);
  }

  return faux;
}
