# Minishell

Minishell est un projet de l'école 42. Le but de ce projet est de créer un interpréteur de commandes simple, similaire à un shell Unix. 

## Description

Dans ce projet, nous avons recréé un shell UNIX de base. Il permet de lire et d'évaluer des commandes simples de l'utilisateur. Les commandes implémentées sont :
- `echo`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`
- l'exécution de commandes externes avec leurs paramètres.

## Utilisation

1. **Compilation** :
   Pour compiler Minishell, naviguez jusqu'au répertoire du projet et utilisez la commande `make`. Cela créera un exécutable appelé `minishell`.

    ```bash
    make
    ```

2. **Exécution** :
   Pour démarrer Minishell, exécutez simplement le fichier binaire créé lors de la compilation :

    ```bash
    ./minishell
    ```

    Une fois le shell lancé, vous pouvez commencer à taper des commandes.

3. **Commandes** :
   Minishell supporte les commandes de base comme `echo`, `cd`, `export`, `unset`, `env`, `exit` et d'autres. De plus, il comprend les redirections (`<`, `>`, `>>`) et les pipes (`|`).

    Par exemple, pour changer le répertoire de travail, utilisez la commande `cd` suivie du chemin du répertoire :

    ```bash
    cd /path/to/directory
    ```

    Pour afficher toutes les variables d'environnement, utilisez la commande `env` :

    ```bash
    env
    ```

    Vous pouvez quitter le shell à tout moment en utilisant la commande `exit` :

    ```bash
    exit
    ```

## Licence

Ce projet est sous licence [MIT](LICENSE).


