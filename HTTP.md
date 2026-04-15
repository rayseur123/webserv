# HTTP: Hyper Text Transfer Protocol

Le protocole http a ete cree dans le but de transmette rapidement et facilement des contenus multimedias
entre un client et un serveur.

## Fonctionnement global:


Le client envoie une requête -> [Methode | URI | Version | Type MIME | Body ]

Ligne-Requête: [Methode SP URI-visée SP Version-HTTP CRLF]

Le serveur envoie une reponse -> [Methode | Code | Message_Code | Version | Type MIME | Body]

Ligne-Etat: [Methode SP URI-visée SP Version-HTTP CRLF]

>Methode: Mot clé permettant de preciser l'action de la requête envoyer (POST , GET, DELETE...)

>Code: Code de succes ou d'erreur (200)

>Message_code: Message permettant d'explique le code (OK)

>URI: (Uniform Resource Identifier) : Emplacement de la resource demandé

>Version: Version de HTTP utilisé (Pas toujours le meme comportement entre deux version)

>Type MIME: Permet de specifié le type de ressource envoyé (Type generale: Type precis) [text:html]

>Body: Les informations a envoyer sur le serveurs (code html)

Difference entre URI / URL:

URI:	Peut etre utiliser dans des cas plus generale (pas uniquement pour le web)
			Permet d'acceder à un fichier sans utiliser son nom

URL:	Surtout utiliser pour le web (Information en plus sur le type de commuication HTPP, FTP)
			Utiliser pour le referencement des sites avec des mots clés



CONNEXION entre le serveur et le client:

Le protocole le + utilisé dans HTTP est le Protocole TCP (Voir autre wiki) 
Permet d'etre sur de la reception de la requête

Le port definis par defaut pour le web est le 80.

La connexion est demandée par le client par l'envoie de la requete
La fermeture de la connexion est faite par le serveur apres l'envoie de la reponse 

Le serveur et le client doivent gerer par leur propre moyen le cas d'une coupure imprevisible
de la communication (par exmple pendant un envoie de reponse). Dans le cas d'un crash pendant 
une reponse alors on arrete tout.

FORMAT HTTP:

CR: retour chariot (\r)
LF: saut de ligne (\n)

Pour chaque nouvelle ligne rajouter a la fin [CRLF] sauf pour le head et le body
Chaque groupe de mot separé par un espace doivent etre mis entre ""

Si [CRLF] ou [LF] est seul alors c'est la fin du header

Exemple:
----------------------------------
|	GET /contact HTTP/1.0\r\n |
|	Host: example.com\r\n					 |
|	User-Agent: Mozilla/5.0\r\n    |
|	\r\n  												 |
----------------------------------

VERSION:

La version permet de comprendre les fonctionalitées autorisée ou non pour la requête/reponse. Si aucune
version n'est specifiée alors on ce basera sur HTTP/0.9 (La plus restrictive). Lorsqu'on passe a des
version posterieur on rajoute seulement des fonctionnalité en plus.

La version se note sous ce format: ["HTTP""/"1*DIGIT"."1*DIGIT]

Le serveur doit repondre sur la meme version que la requête reçu
Gerer les requêtes des version anterieurs au serveur

URI:

URI Absolue: http://host/path
URI Relative: //host/path


Code Erreur:

501: (non implémenté)


Notice for the project:

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








