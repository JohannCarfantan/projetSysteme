#include "execution.h"
#include "commandes_internes.h"
#include "commandes_externes.h"
#define TAILLE_MESSAGE 200

void execution_ligne_cmd(parse_info *info) {

  int i, j, n, nb_arg;
  int resultat;

  /* cet appel est a supprimer et n'est la que pour illustrer l'existence de la structure de donnees */
  //printf("Appel a affiche_parse_info (%s %d) a supprimer\n", __FILE__, __LINE__);
  affiche_parse_info(info);

  i = 0;
  while (i<info->nb_arg) {

    strcpy(info->entree, "");
    strcpy(info->sortie, "");

    /* n aura la valeur de la fin de ligne de commande, ou du debut
     * de la prochaine commande a executer
     */
    n = detection_fin_commande(info, i);

    j = i;
    nb_arg = 0;
    while (j<n) {
      /* Attention cas non detecte arguments apr�s une redirection :
       * commande arg1 < redirection arg2
       * il s'agit a priori d'une erreur de syntaxe du shell
       */
     // printf("info->modificateur[j] : %d \n ",info->modificateur[j]);
      switch(info->modificateur[j]) {
      case REDIRECTION_ENTREE:
	strcpy(info->entree, info->ligne_cmd[j]);
	break;
      case REDIRECTION_SORTIE:
	strcpy(info->sortie, info->ligne_cmd[j]);
	break;
      case ARRIERE_PLAN:
	info->modificateur[i]=ARRIERE_PLAN;
  //printf("info->modificateur[i] : %d \n ",info->modificateur[i]);
	break;
      default:
	nb_arg++;
      }

      j++;
    }

    /* cet appel est a supprimer et n'est la que pour illustrer l'existence de la structure de donnees */
    //printf("Appel a affiche_commande (%s %d) a supprimer\n", __FILE__, __LINE__);
    affiche_commande(info, i);
    printf("\n");

    if(info->modificateur[j]==TUBE) {
      /* il faut traiter (par simplification uniquement pour deux commandes)
       * le cas de la communication via un tube
       */
       execution_cmdPipe();
       return;
    } else {
      resultat = execution_cmd(info, i, nb_arg);
    }

    if(info->modificateur[j]==COMMENTAIRE) {
      /* il suffit de sortir de la fonction, on est arrive a un commentaire
       * le reste de la ligne de commande n'est pas a traiter
       */
       return;
    }

    /* a ce stade, on a le resultat de la commande, et on sait s'il faut traiter la commande
     * suivante en sequence ou avec une condition
     */

    /* traitement post-execution de la partie concernant inf->modificateur[j] != RIEN */
    switch(info->modificateur[j]) {
    case EXECUTION:
      break;

    case EXECUTION_SI:
      /* la/les commande suivante va �tre directement traitee ici :
       * i.e. ne sera pas executee
       */
      if(!resultat) {/* si on doit passer a la commande suivante... */
	while (j<info->nb_arg && \
	       (info->modificateur[j]!=EXECUTION && \
		info->modificateur[j]!=EXECUTION_SINON)) {
	  j++;
	}
      }
      break;
    case EXECUTION_SINON:
      /* la/les commande suivante va �tre directement traitee ici :
       * i.e. ne sera pas executee
       */
      if (resultat) { /* si on doit passer a la commande suivante... */
        while (j<info->nb_arg && \
               (info->modificateur[j]!=EXECUTION && \
          info->modificateur[j]!=EXECUTION_SI)) {
          j++;
        }
      }
      break;
    default :
      /* cas impossible a priori */
      break;
    }

    info->modificateur[j] = AUTRE; /* il suffit d'executer la commande suivante en sequence */
    i = j;
  }
}

t_bool execution_cmd(parse_info *info, int debut, int nb_arg)
{
  if (EST_EGAL(info->ligne_cmd[debut], "echo")) {
    return ActionECHO (info, debut, nb_arg);
  } else if (EST_EGAL (info->ligne_cmd[debut], "cd")) {
    return ActionCD (info, debut, nb_arg);
  } else if (EST_EGAL (info->ligne_cmd[debut], "ls")) {
    return ActionLS (info, debut, nb_arg);
  } else if (EST_EGAL (info->ligne_cmd[debut], "set")) {
    return ActionSET (info, debut, nb_arg);
  } else {
    return ActionEXEC (info, debut, nb_arg);
  }
}

void execution_cmdPipe(parse_info *info, int debut, int nb_arg)
{
    pid_t pid_fils;
    pid_t pid_fils2;
    int descripteurTube[2];
    char messageEcrire[TAILLE_MESSAGE];
    char messageLire[TAILLE_MESSAGE];
    pipe(descripteurTube);
    pid_fils = fork();
    if(pid_fils != 0)
    {
        //ActionEXEC (info, debut, nb_arg);
        sprintf(messageEcrire, "Bonjour, fils. Je suis ton père !");
        write(descripteurTube[1], messageEcrire, TAILLE_MESSAGE);
    }
    else{
      pid_fils2 = fork();
      if(pid_fils2 != 0)
      {
        read(descripteurTube[0], messageLire, TAILLE_MESSAGE);
        info->ligne_cmd[debut+1] = messageLire;
        ActionEXEC (info, debut, nb_arg);
        printf("Message reçu = \"%s\"", messageLire);
      }
    }
}
