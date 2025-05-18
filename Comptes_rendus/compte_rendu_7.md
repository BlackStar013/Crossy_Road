# Compte Rendu réunion 7 : (03/04)

## ODJ :
- point sur le jalon passé
- décisions communes choix implémentation
- jalon suivant

# Jalon passé :
- sprites continués
- structure game {player; map; score; proba; generators; liste_mobs}
+ fonction de move
+ fonction de générateur (utilise énum pour #define 0, 1, ...)
puis prend la proba dans le tableau en comparant la valeur random et les probas
- SDL presque fonctionnel, manque l'avancement de la carte

# Réflexion/choix pour plus tard :
- ajouter un délai pour le mouvement du joueur et des mobs
- pour la sauvegarde, mettre dans un txt qu'on charge

# Prochain jalon :
- tests et début du menu (Louan)
- sprites et biômes (Arthur)
- test_death, generator, plus de mobs et shift game et map (Yovhann)
- fonctions de free d'un peu tout et relier mob et gestion map au jeu (Félix)