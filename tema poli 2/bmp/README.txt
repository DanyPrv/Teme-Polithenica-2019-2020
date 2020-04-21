Enunt problema:
https://ocw.cs.pub.ro/courses/programare/teme_2019/tema3_2019_cbd

Functia readImage:
  - functie ce citeste imaginea si o transmite prin intermediul parametrilor
    primiti prin referinta (fileHeader, infoHeader, bitmap)

Functia printImage:
  - funcie ce primeste ca parametru infoHeaderul, fileHeaderul si matricea de
    pixeli a imaginii si o printeaza

Functia blackAndWite:
  - functie ce primeeste ca parametru calea imaginii si aplica pe aceasta filtrul
    de alb si negru
  - pentru fiecare pixel al imaginii acasta aplica formula prezentata in enunt si
    modifica pixelii imaginii, dupa care o afiseaza si dezaloca spatiul folosit
    pentru stocarea acesteia

Functia noCrop:
  - functie ce primeste ca parametru numele imaginii si o transforma in imagine 
    patratica
  - aceasta trateaza urmatoarele cazuri:
	- cazul in care inaltimea este mai mica decat latimea:
	  -se afiseaza cea de-a doua jumatate de pixeli necesari egalarii dimensiunilor
	  -se afiseaza imaginea initiala
	  -se afiseaza prima jumatate de pixeli necesari egalarii dimensiunilor
	- cazul in care inaltimea este maai mica decat latimea:
	  Pentru fiecare rand al imaginii:
	  -se afiseaza prima jumatate de pixeli necesari egalarii dimensiunilor
	  -se afiseaza imaginea initiala
	  -se afiseaza a doua jumatate de pixeli necesari egalarii dimensiunilor
	- cazul in care cele doua dimensiunisunt egale:
	  -se afiseaza imaginea fara alte modificari
 
Functia convolutionalLayers:
  - functie ce primeste ca parametrii numele imaginii, filtrul si dimensiunea 
    acestuia si aplica taskul 3
  - se creeaza o matrice noua de pixeli pentru a nu strica calitatea calculelor
  - pentru fiecare pixel se aplica filtrul, fiind ignorate zonele inacesibile
    ( de exemplu pixelul [-1][1]) si se tocheaza valoarea calculata in matricea
    secundara
  - se afiseaza imaginea rezultata matricea de pixeli printata fiind cea secundara

Functia maxMinPooling:
  - functie ce primeste ca parametru numele imaginii, dimensiunea filtrului aplicat
    fiecarui pixel si tipul oeratiei (daca Mm este egal cu'm' atunci se aplica 
    minPooling iar daca Mm este 'M' atunci se aplica maxPooling
  - se procedeaza ca la functia anterioara dar se calculeaza minmul sau maximul
    valorilor pixelilor vecini si se modifica pixelul actual pe baza valorii 
    determinate

Functia compare:
  - functie ce primeste ca parametru 2 pixeli si returneaza 1 daca
    valoarea de threshold e mai mica sau egala cu un prag dat, 0 in caz contrar

Functia clustering:
  - functie ce primeste ca paramtetru numele imaginii si valoarea de threshold
    si aplica filtrul de clustering
  - functia ia fiecare pixel de sus in jos si de la stanga la dreapta si verifica 
    daca acesta se afla sau nu intr-o zona (prin intermediul matricii de viztare viz)
  - daca acesta nu apartine unei zone se procedeaza in felul urmator:
	-se creeaza  coada de vecini in care se adauga elementul actual
	-Pentru fiecare pixel din coada:
	  -se verifica daca pixelii vecini sunt compatibii cu functia compare si
           daca nu au fost vizitati se adauga in coada
	-se calculeaza pixelul "mediu" pe baza pixeilor introdusi in coada si se
         seteaza valoarea tuturor pixelilor (aflati in coada) cu valoarea calculata

Functia main:
  - citeste numele pozei, si apeleaza pe rand fiecare din functiile principale de mai sus
