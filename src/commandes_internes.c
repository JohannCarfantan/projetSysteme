#include "divers.h"
#include <dirent.h>
#include "commandes_internes.h"
#include "commandes_externes.h"

t_bool	ActionECHO (parse_info *info, int debut, int nbArg) {

  int i;
  FILE *sortie;
  /* si l'appel est correctement ecrit, on a :
   * arguments[0] == "echo"
   * arguments[1..nbArg-1] = "..."
   *
   * Par de lecture, mais une ecriture redirigee possible
   */

  sortie=stdout;
  //printf("info->sortie : %s \n", info->sortie);

  //Ouverture fichier de sortie '>'
  if (!EST_EGAL(info->sortie, ""))
  {
  sortie=fopen(info->sortie,"w");
  if (sortie==NULL)
  {
    sortie = stdout;
  }
  } 

  i = 1;
  while(i<nbArg)	{
    fprintf(sortie, "%s", info->ligne_cmd[debut+i]);
    i++;
  }
  printf("\n");

  if (sortie!=stdout)
  {
  fclose(sortie);
  }

  return vrai;
}

t_bool	ActionSET (parse_info *info, int debut, int nbArg) {

  /* si l'appel est correctement ecrit, on a :
   * arguments[0] == "set"
   * arguments[1] == "nom_variable"
   * arguments[2] == "="
   * arguments[3] == "valeur"
   * nbArg == 4
   *
   * ou
   * arguments[0] == "set"
   * arguments[1] == "nom_variable"
   * arguments[2] == "="
   * nbArg == 3
   *
   */

  if (!EST_EGAL(info->ligne_cmd[debut], "set") && !EST_EGAL(info->ligne_cmd[debut+2], "=")) {
    printf("Impossible de mettre a jour la variable, appel incorrect\n");
    return faux;
  }

  if (nbArg == 4)	{
    return ecrire_variable (info->ligne_cmd[debut+1], info->ligne_cmd[debut+3]);
  } else if (nbArg == 3) {
    return ecrire_variable (info->ligne_cmd[debut+1], NULL);
  } else {
    return faux;
  }
}

t_bool ActionCD (parse_info *info, int debut, int nbArg) {

  char def[MAX_PATH];
  char dirName[MAX_PATH];
  int i;

  /* le cas nbArg==1 a un comportement par defaut qui mene au
   * repertoire HOME utilisateur
   *
   * Si nbArg>=1 il faut aggreger tous les arguments en un seul chemin
   * (le repertoire de destination contient des espaces)
   */
  if (nbArg == 1) {
    
    lire_variable ("USERPROFILE", def, sizeof (def));
    if (chdir (def) == -1) {
      printf ("Impossible de changer vers le repertoire '%s' \n", def );
      return faux;
    }
    else{
      printf ("Repertoire changé vers : '%s' \n", def);
    }
  } else {

    strcpy(dirName,"");
    for(i=1; i<nbArg; i++) {
      if(strlen(dirName)!=0) strcat(dirName," ");
      strcat(dirName, info->ligne_cmd[debut+i]);
    }

    if (chdir (dirName) == -1) {
      printf ("Impossible de changer vers le repertoire '%s'\n", dirName);
      return faux;
    }
    else{
      printf ("Repertoire changé vers : '%s' \n", dirName);
    }
  }
  return vrai;

}


t_bool	ActionLS (parse_info *info, int debut, int nbArg) {

  /* Utilisation des parametres */
  (void) info;
  (void) debut;
  (void) nbArg;

 // printf("***********************LS***********************\n");

  //Affichage des différentes informations récupérées (Arg)
 // printf("----DEBUT INFO----\n");
  printf("Nombre d'arguments : %d \n", info->nb_arg);
  if ((info->nb_arg)>0)
  {
    for(int i=0; i<info->nb_arg;i++)
    {
      printf("Argument [%d] : %s\n",i, info->ligne_cmd[i]);
    }
  }
  //printf("----FIN INFO----\n");



  

  //printf("----DEBUT EXPLO----\n");

  //Acces au dossier transmis en paramètre
  if ((info->nb_arg)==2)
  {
  DIR *d;
  struct dirent *dir;
  if ((info->nb_arg)>0)
  d = opendir(info->ligne_cmd[1]);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  else{
    printf("Aucune donnée trouvée !\n");
  }
  }else{
    //Si plus de 2 arguments redirection vers commandes_externes
    ActionEXEC (info,debut,nbArg);
  }

  // printf("***********************LS***********************\n");
  
  return faux;
}

