Gabriel Boroghina
Grupa 333CB

                                   New Vegas

    Orasul virtual este generat pe o zona plana patratica de teren, si se afla
inclus intr-o sfera cu diametrul egal cu diagonala patratului terenului ce
va reprezenta cerul (sfera este randata cu o textura corespunzatoare).

---------------------------------------------------------------------------------

    Reteaua de strazi a orasului este una carteziana, cu distante pseudo-random
intre oricare 2 strazi adiacente pe verticala sau pe orizontala. Exista 2 tipuri
de strazi, unul cu trotuare, iar celalalt cu zone verzi pe marginea partii 
carosabile. La intersectia intre oricare 2 strazi este randat un patrat cu
cu o textura de intersectie.

---------------------------------------------------------------------------------

    Cladirile au fost generate pseudo-random, pornind initial de la o inaltime
aleasa random, un numar de fete random (dintre 3, 4, 5, 6 sau 30 - pentru o 
forma cilindrica) si o distanta fixa de la fiecare vertex ce determina sectiunea 
transversala a cladirii pana la centrul acestei sectiuni (sectiunea transversala
reprezinta un poligon inscriptibil intr-un cerc cu raza egala cu aceasta distanta).
    Se construieste initial un poligon regulat cu numarul de fete ales. Apoi se 
aplica un numar de alterari ale unghiurilor dintre segmentele ce unesc centrul
cercului circumscris poligonului cu varfurile acestuia (se ia un numar de grade
de la un unghi ales random si se adauga la alt unghi ales random). Aceasta va
reprezenta sectiunea transversala a nivelului 1 al cladirii.
    Apoi se aplica procesul de asamblare primitive: se replica baza cladirii
(nivelul 1) sub diferite inaltimi si scalari ale bazei (baza isi reduce 
dimensiunile la fiecare nivel superior) si se aseaza nivelurile superioare peste
cele inferioare (prin intermediul unor matrice de modelare).
    Dupa ce cladirea a fost asamblata, se calculeaza o matrice globala de 
transformare care va duce cladirea in interiorul unei celule din grid-ul
determinat de strazi (s-a generat o cladire per celula).
    Cladirile sunt texturate folosind o textura aleasa random dintr-un vector.

---------------------------------------------------------------------------------

    Alte elemente decorative care au fost adaugate in scena sunt: semafoare 
pseudo-random in intersectii (cel mult un semafor per intersectie), copaci dispusi
de o parte sau de alta a strazii, celule ce contin parcuri (zone cu iarba generata
procedural pornind de la un mesh de iarba de dimensiuni reduse) si masini plasate
procedural pe strazi, alese random dintre 4 modele posibile.

---------------------------------------------------------------------------------

    Iluminarea consta intr-un soare global (lumina directionala), precum si in 
spotlights pentru fiecare cladire. 
    S-au generat procedural cate 3 spoturi pentru fiecare cladire, plasate simetric
fata de axa verticala a cladirii, in vecinatatea cercului circumscris sectiunii
transversale a cladirii si cu directia oblica, orientata catre baza si axa verticala
a cladirii.
