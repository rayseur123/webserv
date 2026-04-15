# HTTP: Hyper Text Transfer Protocol

Le protocole http a ete cree dans le but de transmette rapidement et facilement des contenus multimedias
entre un client et un serveur.

## Fonctionnement global:

#### Client
Le client envoie une requête -> **[Methode | URI | Version | Type MIME | Body ]**

Ligne-Requête: **[Methode SP URI-visée SP Version-HTTP CRLF]**  
En-Tête-d'entité: [Seulement pour les methods qui contient un Body]
**SPACE**
Body [Seulement pour les methods qui envoie des infos au serveur]

#### Server
Le serveur envoie une reponse -> **[Methode | Code | Message_Code | Version | Type MIME | Body]**

Ligne-Etat: **["HTTP/" 1*DIGIT "." 1*DIGIT SP 3DIGIT SP]**  
En-Tête-d'entité: [Compris dans la liste ci-dessous] 
	| Content-Encoding
	| Content-Lenght (Obligatoire)
	| Content-Type (Obligatoire)
	| Expires
	| Last-Modified  
**SPACE**  
Body [Seulement pour les methods qui envoie des infos au serveur]  

Body est signalé par la presence de Content-Lenght 

#### Definitions

>**Methode**: Mot clé permettant de preciser l'action de la requête envoyer (POST , GET, DELETE...)  
>**Code**: Code de succes ou d'erreur (200)  
>**Message_code**: Message permettant d'explique le code (OK)  
>**URI**: (Uniform Resource Identifier) : Emplacement de la resource demandé  
>**Version**: Version de HTTP utilisé (Pas toujours le meme comportement entre deux version)  
>**Type MIME**: Permet de specifié le type de ressource envoyé (Type generale: Type precis) [text:html]  
>**Body**: Les informations a envoyer sur le serveurs (code html)

## Methodes

### Post

Permet de soumettre des informations au serveur dans l'endroit indiqué par la requête.


## Difference entre URI / URL:

URI:	Peut etre utiliser dans des cas plus generale (pas uniquement pour le web)
			Permet d'acceder à un fichier sans utiliser son nom

URL:	Surtout utiliser pour le web (Information en plus sur le type de commuication HTPP, FTP)
			Utiliser pour le referencement des sites avec des mots clés



## CONNEXION entre le serveur et le client:

Le protocole le + utilisé dans HTTP est le Protocole TCP (Voir autre wiki) 
Permet d'etre sur de la reception de la requête

Le port definis par defaut pour le web est le **80**.

La connexion est demandée par le client par l'envoie de la requete
La fermeture de la connexion est faite par le serveur apres l'envoie de la reponse 

Le serveur et le client doivent gerer par leur propre moyen le cas d'une coupure imprevisible
de la communication (par exmple pendant un envoie de reponse). Dans le cas d'un crash pendant 
une reponse alors on arrete tout.

## FORMAT HTTP:

>CR: retour chariot (\r)  
LF: saut de ligne (\n)

Pour chaque nouvelle ligne rajouter a la fin **[CRLF]** sauf pour le head et le body
Chaque groupe de mot separé par un espace doivent etre mis entre ""

Si **[CRLF]** ou **[LF]** est seul alors c'est la fin du header

Exemple:

|	GET /contact HTTP/1.0\r\n  
|	Host: example.com\r\n  
|	User-Agent: Mozilla/5.0\r\n  
|	\r\n

## VERSION:

La version permet de comprendre les fonctionalitées autorisée ou non pour la requête/reponse. Si aucune
version n'est specifiée alors on ce basera sur HTTP/0.9 (La plus restrictive). Lorsqu'on passe a des
version posterieur on rajoute seulement des fonctionnalité en plus.

La version se note sous ce format: **["HTTP""/"1*DIGIT"."1*DIGIT]**

Le serveur doit repondre sur la meme version que la requête reçu
Gerer les requêtes des version anterieurs au serveur

URI:

URI Absolue: http://host/path
URI Relative: //host/path


## Code Erreur:

Composé de 3 chiffres, souvent accompagné de la raison qui justifie l'erreur

**1xx** Information	(Non utilisé, pour usage futur)  
**2xx** Succes (L'action a été correctement reçue, interprétée, et exécutée.)  
**3xx** Redirection (Une décision supplémentaire doit être prise pour terminer la requête)  
**4xx** Erreur Client (La requête présente une erreur de forme et ne peut être satisfaite)  
**5xx** Erreur Server (La requête est valide, mais le serveur ne peut la satisfaire)  


	| "201"   ; Created                 Créé
	| "202"   ; Accepted                Accepté
	| "204"   ; No Content              Pas de contenu
	| "301"   ; Moved Permanently       Changement d'adresse définitif
	| "302"   ; Moved Temporarily       Changement temporaire
	| "304"   ; Not Modified            Non modifié
	| "400"   ; Bad Request             Requête incorrecte
	| "401"   ; Unauthorized            Non autorisé
	| "403"   ; Forbidden               Interdit
	| "404"   ; Not 
	| "500"   ; Internal Server Error   Erreur interne serveur
	| "501"   ; Not Implemented         Non implémenté
	| "502"   ; Bad Gateway             Erreur de routeur
	| "503"   ; Service Unavailable     Indisponible
	| autres_codes

## Notice for the project:

Port:
if no specify port also it's 80 for default

URI: gerer les URI absolue et relative ? (normaly not needed to manage query and others)

Version:
Need to manage HTTP/1.0 and 1.1 for cgi.

Date:
Check if we really need to check the date and add the date for the response

Types de media:
Le type, sous-type et les noms d'attributs sont insensibles à la casse. Les valeurs associées aux 
attributs peuvent l'être ou ne pas l'être, suivant la sémantique du nom de l'attribut. 
les types et sous type ne peuvent être séparé par un LWS. Ceci est valable pour les paires 
attribut/valeur Lorsqu'un récepteur reçoit un paramètre irrecevable pour le type de média spécifié, 
il devra traiter le descripteur comme si cette paire attribut/valeur n'existait pas..

Canonisation texte par default:
Les applications HTTP doivent accepter des fins de ligne représentées par la séquence CRLF,
par un CR seul, ou par un LF seul, dès qu'il s'agit d'un média "text"

Faut-il faire les requêtes simple en plus des complexes?

URI VISÉE:
est transmise sous forme d'une chaîne encodée, dans laquelle certains caractères apparaîtront comme 
une séquence d'échappement "% HEX HEX" telle que définie par la RFC 1738 [4]. Le serveur origine devra 
décoder cette chaîne afin d'accéder correctement à la ressource.

7.2.2 Longueur
Lorsqu'un corps d'entité est présent dans un message, la longueur de ce corps doit être explicitée par l'un des deux 
moyens suivants. Si un champ Content-Length est présent, sa valeur associée représente la longueur en octets du corps d'entité.
Autrement, c'est la déconnexion par le serveur qui marque la fin du corps d'entité.

Surement la reponse a la question precedent ! -> ce dernier répondra par un message de code 400 (erreur client).

## METHODE POST 

La fonction effective de la méthode POST est définie par le serveur et dépend de l'URI désignée. L'entité "postée" est
subordonnée àcette URI dans le même sens qu'un fichier est subordonné au répertoire qui le contient, un article est subordonné 
au groupe de nouvelles à qui il a été posté, ou un enregistrement à une base de données.

La résolution d'une méthode POST ne signifie pas forcément la création d'une entité sur le serveur origine, ni une 
possibilité d'accès futur à ces informations. En d'autres termes, le résultat d'un POST n'est pas nécessairement une 
ressource associable à une URI. Dans ce cas, une réponse de code 200 (OK) ou 204 (pas de contenu) est la réponse la plus 
appropriée, suivant que la réponse contient ou non une entité pour décrire le résultat.

Si une ressource a été créée sur le serveur origine, la réponse sera du type 201 (créé) et contiendra l'entité 
(de préférence du type "text/html") qui décrira les informations sur la requête et contiendra une référence sur la nouvelle 
ressource

Un champ Content-Length valide est demandé dans toute requête POST HTTP/1.0. Un serveur HTTP/1.0 répondra par un message 
400 (requête incorrecte) s'il ne peut déterminer la longueur du contenu du message







