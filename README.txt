PÁRHUZAMOS PROGRAMOZÁS PROJEKT
Dns szekvenciálás lehetséges megoldása párhuzamosított mintaillesztési algoritmusokkal
PROJEKTHEZ BEADOTT ISMERTETÕ
Az általam választott alkalmazás, egy DNS szekvenciálási felhasználásra fejlesztett, karakterlánc mintaillesztõ alkalmazás, melynek célja a Knuth-Morris-Pratt, a Boyer-Moore és a Rabin-Karp algoritmusok párhuzamosítása. 
A projekt célja, hogy a DNS szekvencia kettõs láncolt lista ábrázolásának minél gyorsabb feldolgozása, illetve adott minta megtalálása a szekvenciákban.
Egy DNS szekvencia közel 3 milliárd vagyis 3*10^9  adatot tartalmaz, melyek a heterociklusos bázis párok (A adenin, guanin G, citozin C, timin T) listájával ábrázolható , melyek a következõ páronként jelennek meg közel arányos eloszlásban: A-T, G-C. 
Az alkalmazás az alábbi algoritmusokat fogja tartalmazni:
* Egy általam fejlesztett GPU algoritmus, mely legenerálja a szekvenciát
* Knuth-Morris-Pratt (KMP) algoritmus CPU-ra megírt verziója.
* Boyer-Moore (BM) algoritmus CPU-ra megírt verziója. 
* Rabin-Karp (RK) algoritmus CPU-ra megírt verziója. 
* Knuth-Morris-Pratt algoritmus GPU-ra megírt verziója.
* Rabin-Karp algoritmus GPU-ra megírt verziója. 
A legtöbb számítási idõt a CPU-ra megírt algoritmusok fogják elhasználni tekintve, hogy mindegyik algoritmus -kis heurisztikával is- de lineáris elven mûködnek, illetve a KMP és BM algoritmusok elõkészítési algoritmusa magas lépésszámú lesz A láncolt vektor feldolgozása és a mintaillesztés((KMP,BM)) a vektor összeadáshoz hasonló módszerrel jól párhuzamosítható.  A RB algoritmus jelen alkalmazási elv szerint mentesül a Horner elrendezés algoritmus lefuttatása alól, mivel teljes ABC helyett csak 4 karakterünk van, tehát szintén könnyen párhuzamosítható. 



A MEGVALÓSÍTOTT PROJEKT
A projekt kiválasztásakor, nem vettem figyelembe, hogy mekkora idõigény szükséges az algoritmusok helyes párhuzamosításához, és a párhuzamosítás implementálásához. Emiatt nem készült el a projekt határidõre, és nem lett volna kész a pót határidõre sem. Alábecsültem a feladat bonyolultságát.
Ebbõl az okból kifolyólag redukáltam a projektet, hogy az elkészülhessen a póthatáridõre. Az így megvalósított projekt leírása a következõ. Elkészült a párhuzamosított DNS szekvencia generátor, mely 3 kernel metódust tartalmaz. (részletezés a késõbbiekben) Illetõleg elkészült a 3 választott algoritmus implementálása processzorra.
A projekt ugyan nem készült el, de a generate függvény teljes mértékben kimeríti a kurzuson tanultak alkalmazását, így alkalmas az értékelés meghatározására.
IMPLEMENTÁCIÓ
SZERKEZETI VÁZLAT
A megvalósított alkalmazás fõbb elemei a következõk:
* generate.h
o A DNS szekvencia generálás párhuzamosított algoritmusát tartalmazza
* CPU_search.h
o Tartalmazza a 3 mintaillesztési algoritmus szekvenciális változatát
* KMP
* BM
* RK
* GPU_search.h
o Tartalmazza a 3 mintaillesztési algoritmusból kettõnek a párhuzamosítására tett próbálkozásaimat.
* KMP
* BM ( nem tartalmazza)
* RK
* DNA_seq.cu
o Az alkalmazás main metódusát tartalmazza.



AZ ALGORITMUSOK ISMERTETÉSE
KMP algoritmus
A Knuth-Morris-Pratt eljárásnak a Brute-Force (hasonlítsuk össze, toljunk egyet, stb..) módszerrel szemben az az elõnye, hogy egyes esetekben, ha a mintában vannak ismétlõdõ elemek, akkor egy tolásnál akár több karakternyit is ugorhatunk.

1. ábra KMP algoritmus több karakter tolás esetén
Az ugrás megállapítását a következõképp tesszük: Az eddig megvizsgált egyezõ mintarész elején (prefix) és végén (suffix) olyan kartersorozatot keresünk, melyek megegyeznek. Ha találunk ilyet, akkor a mintát annyival tolhatjuk, hogy az elején lévõ része ráilleszkedjen a végen levõre.
Azt, hogy ez egyes esetekben mekkorát tolhatunk nem kell minden elromlás alkalmával vizsgálni. Ha a mintára ¨önmagával lefuttatjuk az algoritmus egy módosított változatát (3. ´ábra), kitölthetünk egy tömböt, mely alapján a tolásokat végezni fogjuk.

2. ábra: KMP tolásokat szabályzó tömb kitöltésé
Az algoritmus (ld 2. ábra):
Két indexet i ´es j futtatunk a szövegen illetve a mintán.
Ha az i + 1-edik ´es j + 1-edik karakterek megegyeznek, akkor léptetjük mind a kettõt.
Ha nem egyeznek meg, akkor:
Ha a minta elsõ elemét vizsgáltuk, akkor egyet tolunk a mintán, magyarul a minta indexe marad az elsõ betûn, ´es a szövegben lévõ indexet növeljük eggyel (i = i + 1)
Ha nem a minta elsõ elemét vizsgáltuk, akkor annyit tolunk, amennyit szabad. Ez azt jelenti, hogy csak a mintán lévõ indexet helyezzük egy kisebb helyre (j = next[j])
Addig megyünk, míg vagy a minta, vagy a szöveg végére nem ´érünk. Ha a minta végére ´értünk, akkor megtaláltuk a mintát a szövegben, ha a szöveg végére ´értünk, akkor pedig nem.
BM algoritmus (Quick Search)
Míg a KMP algoritmus az elromlás helye elõtti rész alapján döntött a tolásról, addig a QS a minta utáni karakter alapján. Tehát elromlás esetén:
• Ha a minta utáni karakter benne van a mintában, akkor jobbról az elsõ elõfordulására illesztjük


3. ábraQS - eltolás ha a minta utáni karakter benne van a mintában • Ha a minta utáni karakter nincs benne a mintában, akkor a mintát ezen karakter után illesztjük.

4. ábra: QS - eltolás ha a minta utáni karakter nincs benne a mintában
Az eltolás kiszámítását megint elõ lehet segíteni egy tömbbel, most azonban, mivel nem a minta az
´érdekes, ´es nem tudjuk pontosan mely karakterek szerepelnek a szövegben, így a tömbbe az egész abc-t fel kell vennünk (7. ´ábra)

5. ábra: QS - Az eltolást elõsegítõ tömb (Shift[0a0...0z0]) konstruálása

Két indexet k ´es j futtatunk a szövegen illetve a mintán.
Ha a szöveg k + j-edik eleme megegyezik a minta j-edik karakterével, akkor léptetjük j-t (mivel a szövegben k + j-edik elemet nézzük, ´így elég j-t növelni).
Ha nem egyeznek meg, akkor:
Ha a minta már a szöveg végén van (k = n?m), akkor csak növeljük k-t eggyel, ami hamissá teszi a ciklus feltételt.
Ha még nem vagyunk a szöveg végén k-t toljuk annyival, amennyivel lehet (ezt az elõre beállított Shift tömb határozza meg). Es a´ j-t visszaállítjuk 1-re.
Addig megyünk, míg vagy a minta végére ´érünk j-vel, vagy a mintát tovább toltuk a szöveg végénél.
Elõbbi esetben egyezést találtunk, míg az utóbbiban nem.

6. ábra QS
RK algoritmus
A Rabin-Karp algoritmus lényege, hogy minden betûhöz az ´abécébõl egy számjegyet rendelünk, ´es a keresést számok összehasonlításával végezzük. Világos, hogy ehhez egy ´ábécé méretnek megfelelõ számrendszerre lesz szükségünk. A szövegbõl mindig a minta hosszával egyezõ részeket szelünk ki, ´es ezeket hasonlítjuk ¨össze.
Példa:
Minta: BBAC ? 1102
Szöveg: DACABBAC ? 30201102, amibõl a következõ számokat ´állítjuk elõ: 3020, 0201, 2011, 0110, 1102
A fent látható szeletek lesznek az si-k.
Az algoritmus mûködéséhez azonban számos apró ¨ötletet alkalmazunk:
A minta számokká alakítását Horner-módszer segítségével végezzük.

7. ábra: RK - Horner-módszer
Az ord() függvény az egyes betûknek megfelelõ számot adja vissza. A d a számrendszer alapszáma.
A szöveg mintával megegyezõ hosszú´ szeletéinek (si) elõállítása: s0-t a Horner-módszerrel ki tudjuk számolnia. Ezek után si+1 a következõképp számolandó: si+1 = (si ? ord(S[i]) · dm?1) · d + ord(S[i + 1]) Felmerülhet a kérdés, hogy az ilyen magas alapszámú´ számrendszerek nem okoznak-e gondot az ábrázolásnál? A kérdés jogos. Vegyük a következõ ´életszerû½ példát: 4 bájton ´ábrázoljuk a számainkat (232). Az abc legyen 32 elemû½ (d = 32), a minta 8 hosszú´ (m = 8). Ekkor a dm?1 kiszámítása: 327 = (25)7 = 235 , ami már nem ´ábrázolható 4 bájton. Ennek kiküszöbölésére vezessünk be egy nagy p prímet, melyre d · p még ´ábrázolható. Es a´ mûveleteket számoljuk mod p. Ekkor természetesen a kongruencia miatt lesz olyan eset, amikor az algoritmus egyezést mutat, mikor valójában nincs. Ez nem okoz gondot, mivel ilyen esetben karakterenkénti egyezést vizsgálva ezt a problémát kezelni tudjuk. (Fordított eset nem fordul elõ tehát nem lesz olyan eset, mikor karakterenkénti egyezés van, de numerikus nincs). [Ha p kellõen nagy, a jelenség nagyon ritkán fordul elõ.]
A mod p számítás egy másik problémát is felvet. Ugyanis a kivonás alkalmával negatív számokat is kaphatunk.
Például: Legyen p = 7, ekkor, ha ord(S[i]) = 9, akkor elõzõ számítás után si = 2..., de ebbõl ord(S[i]) · dm?1 = 9 · 103 = 9000-et vonunk ki negatív számot kapunk. Megoldásként si+1-et két lepésben számoljuk:
	s := (si + d · p ? ord(S[i]) · dm?1)	mod p
	si+1 := (s · d + ord(S[i + 1]))	mod p
A fentiek alapján az algoritmus a következõ (ld. 8. ´ábra)
* Kiszámoljuk dm?1-et (dm1)
* Egy iterációban meghatározzuk Horner-módszerrel a minta számait (x) ´es s0-t
* Ellenõrizzük, hogy egyeznek-e
* Addig számolgatjuk si ´érteket míg a minta nem egyezik si-vel, vagy a minta a szöveg végére nem ért.
8. ábra: RK



GENERATE.H
A generate.h fejállomány 3 párhuzamosított metódust tartalmaz, melyek teljes mértékben a saját általam kitalált és lefejlesztett algoritmusokat tartalmazzák. Továbba egy szekvenciális kiirató metódust, a generálást megvalósító generate függvényt, és a szokásos órán használt cuda error ellenõrzõ metódust.
GENERATE_KERNEL
A metódus paraméterül várja a két még üres szekvenciát, azok méretét, n illetve egy curand generált unsigned int tömböt mely a genomok eloszlását fogja megadni. A genom tömb tartalmazza a 4 karaktert amit egy genom tartalmazhat. Az elsõ szekvenciába kerülnek a random kapott genom tömb elemek, a második szekvenciába kerülnek a random kapott elem ellentétei egy switch-case segítségével. Ezek után a metódus egy random helyen elrejti a mutációt, a paraméteról kapott random tömb alapján ami nem mellesleg az implementáció szerint Poisson eloszlású.
HIDE(KERNEL)
A hide metódus végzi a pattern elrejtését a szekvenciák valamelyikében, ehhez paraméterül várja két random számot, hogy melyik szekvenciában és melyik karakternél(hanyadik elemnél) kezdje el. innentõl a metódus azonos elven mûködik mint a generate függvény, vagyis switch – case-ek segítségével módosítja egyszerre a két tömböt.
PRINT_DNA_KERNEL
Egy egyszerû párhuzamosított tömb kiirató metódus, melyhez a lépéseket stride-ok segítségével implementáltam.
GENERATE METÓDUS
A generate metódus végzi a memória foglalásokat a GPU részére, a hide kernelhez szükséges random számok, illetve a curand poisson tömb generálását. A szükséges memória másolások utén futtatja a 3 fent említett metódus futtatását és szinkronizálását.


GPU_SEARCH.H
Mint ahogy a „A megvalósított projekt” fejezetben már leírtam, nem sikerült a keresések párhuzamosítása, a header fájl fordul, de a keresés nem talál eredményt a szekvenciában. Az algoritmusok párhuzamosításához két a github-on fellelhetõ implementációt vettem alapul, de átírásuk nem érte el a kellõ sikert. A KMP algoritmushoz, a https://github.com/Fang-Haoshu/KMP-on-CUDA oldalon található fájlokat kértem segítségül. Míg az RK algoritmushoz a https://github.com/mstftrn/rabin-karp-for-CUDA repót használtam fel.
CPU_SEARCH
A CPU_seearch fejállomány tartalmazza a CPU keresésekhez szükséges algoritmusokat, melyek megfelelnek az algoritmusok elõzõekben megadott leírásának. A gyors haladás érdekében, és mert a Tanár úr is javasolta, a CPU algoritmusokat, már kész implementációk alapján készítettem.
A KMP algoritmust
https://www.techiedelight.com/implementation-kmp-algorithm-c-cpp-java/,
A QS algoritmust
https://www.geeksforgeeks.org/boyer-moore-algorithm-good-suffix-heuristic/
Az RK algoritmust
https://www.geeksforgeeks.org/rabin-karp-algorithm-for-pattern-searching/
alapján implementáltam a feladathoz.
FORDÍTÁS ÉS FUTTATÁS
FORDTÁS
A programot a -lcurand kapcsolóval kell fordítani a generate.h állományban található curand random generátor miatt. A fordítás teljes parancsa: nvcc DNA_seq.cu -lcurand -o dna
FUTTATÁS
Innentõl a szokásos módon futtatható az állomány, viszont az átláthatóság érdekében érdemes egy txt fájlba futtatni, pontosabban: ./dna > result.txt
MEGJEGYZÉS
A DNA_GPU.cu nevû fájllal futtathatóak az algoritmusok futtatására tett próbálkozásaim.
A CPU_search.h és a GPU_search.h együtt nem include-olható, ezért került külön fájlba.
A helyes parancs: nvcc DNA_GPU.cu -lcurand -o dnagpu.
A futtatás az elõzõekkel megegyezõ.
A szekvenciák mérete növelhetõ, de a gyors futtatás végett, nem ajánlott(A CPU keresések nagy méretû tömbön lassúak.) Továbbá az elrejtést hozzá kell igazítani a tömbhöz:
int j = (i-1)%PATTERN_SIZE; // if the sequence size is 15000 i+2


