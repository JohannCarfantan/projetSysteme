#include "divers.h"
#define KBLU  "\x1B[36m"
#define KWHT  "\x1B[0m"

void AfficheInvite() {

  char invite[CHAINE_MAX];
  char chaine[CHAINE_MAX];
  char var[CHAINE_MAX];
  size_t i;
  char * dwRet;
  if (lire_variable ("INV", invite, sizeof (invite))) {


    /* Format reconnu :
     * \u : remplace par l'utilisateur
     * \h : remplace par le nom de machine
     * \p : remplace par le chemin courant
     */

    strcpy(chaine, "");
    for(i=0; i<strlen(invite); i++) {
      if (invite[i]=='\\' && i<strlen(invite)-1) {
	i = i+1;
	switch(invite[i]) {
	case 'u' :
	  lire_variable ("USERNAME", var, sizeof (var));
	  break;
	case 'h' :
	  lire_variable ("COMPUTERNAME", var, sizeof (var));
	  break;
	case 's' :
	  strcpy(var," ");
	  break;
	case 'p' :
	  dwRet = getcwd (var, sizeof (var));

	  if (dwRet == NULL) {
	    fprintf (stderr, "Echec lors de l'appel a getcwd !\n");
	    fflush (stderr);
	  }
	  break;
	default :
	  /* cas impossible a priori */
	  break;
	}
      }
      else {
	  sprintf(var, "%c", invite[i]);
      }
      strncat(chaine, var, CHAINE_MAX - strlen(chaine));
    }

  }
  else {
    strcpy(chaine, "$ ");
  }

  //printf("%s",chaine);

  //Ecriture dans stdout du répertoire courant car problème avec le code du haut 
  dwRet = getcwd (var, sizeof (var));
  if(dwRet!=NULL)
  {
   strcpy(chaine,dwRet);
   strcat(chaine, "$ : ");
  }
  else{
  strcpy(chaine, "$ ");
  }
  

  printf ("%s%s%s",KBLU ,chaine,KWHT);
  fflush (stdout);
}

t_bool ecrire_variable (char* nomVar, char* valeur) {

  /*printf("Appel a ecrire_variable (%s %d) a ecrire avec \"%s\" et \"%s\". \n",
	 __FILE__,
	 __LINE__,
	 nomVar,
	 valeur);*/
	setenv(nomVar,valeur,1);
	//printf("Resultat : %s\n",getenv(nomVar));
  return faux;
}

t_bool lire_variable (char* nomVar, char* valeur, int taille) {
 
   strcpy(valeur, "");

  if (nomVar != NULL) {
    char* var = getenv(nomVar);
    if (var == NULL) {
     // perror("getenv");
    }
    else{
      	strcpy(valeur, var);
    }
  }
  
  (void) taille;

  /*printf("Appel a lire_variable (%s %d) a ecrire avec \"%s\", \"%s\" et %d. \n",
	 __FILE__,
	 __LINE__,
	 nomVar,
	 valeur,
	 taille);*/
  return faux;
}
