Enunt problema:
https://ocw.cs.pub.ro/courses/programare/teme_2019/tema2_licurici


Functia main:
  Seciteste instructiunea curenta iar cat timp aceasta nu este "quit" se apeleaza 
  functiile specifice fiecarei instrunctiuni.

Functia uppercase:
  functie ce primeste ca parametru poezia, parcuge fiecare litera si le trnasforma
  in litere mici in afara de cazul in care aceasta este la inceput de rand.

Functia trimming:
  functie ce prieste ca parametru poezia, o supracsrie cu ajutorul a 2 indici 
  (la pozitia j se va scrie litera de pe pozitia i).
  In cazul in case se intalneste, o litera, caracterul de linie noua, sau 
  caracterul '-' ce se afla intre doua litere, se scrie la pozitia j elementul
  de pe pozitia i si se incrementeaza j
  In cazul in care se intalneste ' ' se verifica daca caracterul anterior este
  un spatiu iar daca nu este se acesta scrie pe pozitia j, se incrementeaza j
  si whiteSpace devine 1.
  In orice alt caz j nu se incrementeaza.

Functia friendly:
  Functire ce primeste ca parametru poezia, o copiaza intr-un vector secundar
  (str) si incepe extragerea fiecarui cuvant.
  Pentru fiecare cuvant se cauta cu ajutorul functiei get_friendly_word daca 
  are diminutiv.
  Daca acesta are, se memoreaza (in aux) tot ce urmeaza dupa cuvantul ce 
  trebuie inlocuit, se sterge tot ce este dupa acesta din poezie, inclusiv 
  cuvantul, in urma stergerii se adauga la poezie noul cuvant (diminutivul)
  si tot ce urma dupa vechiul cuvant.

Functia print:
  Functie ce primeste ca parametru poezia si o afiseaza in consola.

Functia isLetter:
  Functie ce primeste ca parametru un caracter si returneaza 1 daca 
  acesta este o litera, 0 in caz contrar.

Functia isLower:
  Functie ce primeste ca parametru un caracter si returneaza 1 daca
  acesta este o litera mica, 0 in caz contrar. 