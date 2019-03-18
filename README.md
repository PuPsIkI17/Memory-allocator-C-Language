# Memory-allocator-C-Language
Pislari Vadim -> Facultatea Automatica si Calculatoare -> Universitatea Politehnica Bucuresti

                                                 (Romanian language)
                                               -------DESCRIERE-------

Proictul realizeaza o simulare a unui sistem de alocare de memorie. Programul primeste la intrare comenzi de alocare, alterare, afişare şi eliberare de memorie, şi furnizeaza la ieşire rezultatele fiecărei comenzi. 

Funcţiile unui alocator de memorie:
Un alocator de memorie poate fi descris, în termenii cei mai simpli, în felul următor:
Primeşte un bloc mare, compact (fără “găuri”), de memorie, pe care trebuie să-l administreze. Utilizatorii cer din acest bloc, porţiuni mai mici, de dimensiuni specificate. Alocatorul gaseste în arenă o porţiune continuă liberă (nealocată), de dimensiune mai mare sau egală cu cea cerută de utilizator, pe care apoi o marchează ca ocupată şi întoarce utilizatorului adresa de început a zonei proaspăt marcată drept alocată. Alocatorul are grijă ca blocurile alocate să nu se suprapună (să fie disjuncte), pentru că altfel datele modificate într-un bloc vor altera şi datele din celălalt bloc.
Utilizatorii pot apoi să ceară alocatorului să elibereze o porţiune de memorie alocată în prealabil, pentru ca noul spaţiu liber să fie disponibil altor alocări.
La orice moment de timp, arena arată ca o succesiune de blocuri libere sau ocupate.Arena se considera o succesiune(vector) de N octeţi.
Fiecare bloc alocat de memorie constă din două secţiuni:

Prima secţiune, de gestiune, este reprezentată de 12 octeţi (3 * sizeof(uint32_t)) împărţiţi în 3 întregi (a câte 4 octeţi fiecare). Cei trei întregi reprezintă următoarele:
Primul întreg reprezintă indexul de start al blocului următor de memorie din arenă. 
Cel de-al doilea întreg din secţiune reprezintă indexul de start al blocului imediat anterior din arenă. Dacă blocul este primul în arenă, atunci valoarea acestui este 0.
Cel de-al treilea întreg din secţiune reprezintă lungimea sectiunii de date.
A doua secţiune conţine datele efective ale utilizatorului. Secţiunea are lungimea în octeţi egală cu dimensiunea datelor cerută de utilizator la apelul funcţiei de alocare. 

Proiecul isi propune implementarea o serie de operaţii de lucru cu arena, care sunt lansate în urma comenzilor pe care le primeşte la intrare. 

INITIALIZE <N>
Această comandă realizeaza iniţializarea unei arene de dimensiune N octeţi. Prin iniţializare se înţelege alocarea dinamică a memoriei necesare stocării arenei, setarea fiecărui octet pe 0, şi iniţializarea lanţului de blocuri.

FINALIZE
Această comandă eliberereaza memoria alocată la iniţializare.

DUMP
Această comandă afiseaza întreaga hartă a memoriei, aşa cum se găseşte în acel moment, octet cu octet. 

ALLOC <SIZE>
Comanda aloca SIZE octeţi de memorie din arenă. Ea gaseste o zonă liberă suficient de mare (care să încapă SIZE octeţi + secţiunea de gestiune), şi rezerva un bloc 'la începutul' zonei. 

FREE <INDEX>
Comanda elibereaza blocul de memorie al cărei secţiuni de date începe la poziţia INDEX în arenă.
  
FILL <INDEX> <SIZE> <VALUE>
Comanda seteaza SIZE octeţi din arenă la valoarea VALUE, cuprinsă între 0 şi 255 inclusiv, si modifica octetii blocurilor începând cu blocul cu indexul INDEX. 
