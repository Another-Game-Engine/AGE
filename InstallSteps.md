###Etape 1) Le Projet
Cloner le repos https://github.com/Another-Game-Engine/AGE

###Etape 2) Les Vendors
Telecharger les vendors ici : https://drive.google.com/folderview?id=0B6r28mwzes4tdHNHZjU3LW1wVmM&usp=sharing
Les mettres à la racine du projet

###Etape 3) Les Assets
Cloner le repos https://github.com/Another-Game-Engine/AGE-Assets-For-Test dans un dossier Assets à la racine du projet

###Etape 4) Configurer le projet
- Lancer le .sln à la racine du projets
- Mettre le projet en 64bits
- Properties - > debugging - > working directory -> $(SolutionDir)$(Platform)\$(Configuration)\
- EngineCoreTest - > set as startup Project
- En fonction de la version de openGL - > modifier les fichiers de shader, et les preshader  de #version 440 à #version 330 

C'est bon !!!
