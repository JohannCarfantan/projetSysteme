#include "divers.h"
#include "commandes_externes.h"
#include <sys/wait.h>

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

    for (int i=0;i<nbArg;i++)
    {
    printf("arg : %s \n", arg[i]);
    }


   pid_t pid_fils = fork();
    if ( pid_fils == -1) {
    perror ("Echec du fork 1\n");
    return EXIT_FAILURE;
    }
    if( pid_fils ==0){
      int res = execvp(*(&arg[0]),&arg[1]);
      (void) res;
      //printf("INT res : %d \n", res);
    }

  //printf("execution d'une commande externe  (%s %d) a ecrire :\n%s\n", __FILE__, __LINE__, ligne);
  (void) premierPlan;


  wait(NULL);

  return faux;
}
