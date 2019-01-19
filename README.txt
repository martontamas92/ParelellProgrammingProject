P�RHUZAMOS PROGRAMOZ�S PROJEKT
Dns szekvenci�l�s lehets�ges megold�sa p�rhuzamos�tott mintailleszt�si algoritmusokkal
PROJEKTHEZ BEADOTT ISMERTET�
Az �ltalam v�lasztott alkalmaz�s, egy DNS szekvenci�l�si felhaszn�l�sra fejlesztett, karakterl�nc mintailleszt� alkalmaz�s, melynek c�lja a Knuth-Morris-Pratt, a Boyer-Moore �s a Rabin-Karp algoritmusok p�rhuzamos�t�sa. 
A projekt c�lja, hogy a DNS szekvencia kett�s l�ncolt lista �br�zol�s�nak min�l gyorsabb feldolgoz�sa, illetve adott minta megtal�l�sa a szekvenci�kban.
Egy DNS szekvencia k�zel 3 milli�rd vagyis 3*10^9  adatot tartalmaz, melyek a heterociklusos b�zis p�rok (A adenin, guanin G, citozin C, timin T) list�j�val �br�zolhat� , melyek a k�vetkez� p�ronk�nt jelennek meg k�zel ar�nyos eloszl�sban: A-T, G-C. 
Az alkalmaz�s az al�bbi algoritmusokat fogja tartalmazni:
* Egy �ltalam fejlesztett GPU algoritmus, mely legener�lja a szekvenci�t
* Knuth-Morris-Pratt (KMP) algoritmus CPU-ra meg�rt verzi�ja.
* Boyer-Moore (BM) algoritmus CPU-ra meg�rt verzi�ja. 
* Rabin-Karp (RK) algoritmus CPU-ra meg�rt verzi�ja. 
* Knuth-Morris-Pratt algoritmus GPU-ra meg�rt verzi�ja.
* Rabin-Karp algoritmus GPU-ra meg�rt verzi�ja. 
A legt�bb sz�m�t�si id�t a CPU-ra meg�rt algoritmusok fogj�k elhaszn�lni tekintve, hogy mindegyik algoritmus -kis heurisztik�val is- de line�ris elven m�k�dnek, illetve a KMP �s BM algoritmusok el�k�sz�t�si algoritmusa magas l�p�ssz�m� lesz A l�ncolt vektor feldolgoz�sa �s a mintailleszt�s((KMP,BM)) a vektor �sszead�shoz hasonl� m�dszerrel j�l p�rhuzamos�that�.  A RB algoritmus jelen alkalmaz�si elv szerint mentes�l a Horner elrendez�s algoritmus lefuttat�sa al�l, mivel teljes ABC helyett csak 4 karakter�nk van, teh�t szint�n k�nnyen p�rhuzamos�that�. 



A MEGVAL�S�TOTT PROJEKT
A projekt kiv�laszt�sakor, nem vettem figyelembe, hogy mekkora id�ig�ny sz�ks�ges az algoritmusok helyes p�rhuzamos�t�s�hoz, �s a p�rhuzamos�t�s implement�l�s�hoz. Emiatt nem k�sz�lt el a projekt hat�rid�re, �s nem lett volna k�sz a p�t hat�rid�re sem. Al�becs�ltem a feladat bonyolults�g�t.
Ebb�l az okb�l kifoly�lag reduk�ltam a projektet, hogy az elk�sz�lhessen a p�that�rid�re. Az �gy megval�s�tott projekt le�r�sa a k�vetkez�. Elk�sz�lt a p�rhuzamos�tott DNS szekvencia gener�tor, mely 3 kernel met�dust tartalmaz. (r�szletez�s a k�s�bbiekben) Illet�leg elk�sz�lt a 3 v�lasztott algoritmus implement�l�sa processzorra.
A projekt ugyan nem k�sz�lt el, de a generate f�ggv�ny teljes m�rt�kben kimer�ti a kurzuson tanultak alkalmaz�s�t, �gy alkalmas az �rt�kel�s meghat�roz�s�ra.
IMPLEMENT�CI�
SZERKEZETI V�ZLAT
A megval�s�tott alkalmaz�s f�bb elemei a k�vetkez�k:
* generate.h
o A DNS szekvencia gener�l�s p�rhuzamos�tott algoritmus�t tartalmazza
* CPU_search.h
o Tartalmazza a 3 mintailleszt�si algoritmus szekvenci�lis v�ltozat�t
* KMP
* BM
* RK
* GPU_search.h
o Tartalmazza a 3 mintailleszt�si algoritmusb�l kett�nek a p�rhuzamos�t�s�ra tett pr�b�lkoz�saimat.
* KMP
* BM ( nem tartalmazza)
* RK
* DNA_seq.cu
o Az alkalmaz�s main met�dus�t tartalmazza.



AZ ALGORITMUSOK ISMERTET�SE
KMP algoritmus
A Knuth-Morris-Pratt elj�r�snak a Brute-Force (hasonl�tsuk �ssze, toljunk egyet, stb..) m�dszerrel szemben az az el�nye, hogy egyes esetekben, ha a mint�ban vannak ism�tl�d� elemek, akkor egy tol�sn�l ak�r t�bb karakternyit is ugorhatunk.

1. �bra KMP algoritmus t�bb karakter tol�s eset�n
Az ugr�s meg�llap�t�s�t a k�vetkez�k�pp tessz�k: Az eddig megvizsg�lt egyez� mintar�sz elej�n (prefix) �s v�g�n (suffix) olyan kartersorozatot keres�nk, melyek megegyeznek. Ha tal�lunk ilyet, akkor a mint�t annyival tolhatjuk, hogy az elej�n l�v� r�sze r�illeszkedjen a v�gen lev�re.
Azt, hogy ez egyes esetekben mekkor�t tolhatunk nem kell minden elroml�s alkalm�val vizsg�lni. Ha a mint�ra ��nmag�val lefuttatjuk az algoritmus egy m�dos�tott v�ltozat�t (3. ��bra), kit�lthet�nk egy t�mb�t, mely alapj�n a tol�sokat v�gezni fogjuk.

2. �bra: KMP tol�sokat szab�lyz� t�mb kit�lt�s�
Az algoritmus (ld 2. �bra):
K�t indexet i �es j futtatunk a sz�vegen illetve a mint�n.
Ha az i + 1-edik �es j + 1-edik karakterek megegyeznek, akkor l�ptetj�k mind a kett�t.
Ha nem egyeznek meg, akkor:
Ha a minta els� elem�t vizsg�ltuk, akkor egyet tolunk a mint�n, magyarul a minta indexe marad az els� bet�n, �es a sz�vegben l�v� indexet n�velj�k eggyel (i = i + 1)
Ha nem a minta els� elem�t vizsg�ltuk, akkor annyit tolunk, amennyit szabad. Ez azt jelenti, hogy csak a mint�n l�v� indexet helyezz�k egy kisebb helyre (j = next[j])
Addig megy�nk, m�g vagy a minta, vagy a sz�veg v�g�re nem ��r�nk. Ha a minta v�g�re ��rt�nk, akkor megtal�ltuk a mint�t a sz�vegben, ha a sz�veg v�g�re ��rt�nk, akkor pedig nem.
BM algoritmus (Quick Search)
M�g a KMP algoritmus az elroml�s helye el�tti r�sz alapj�n d�nt�tt a tol�sr�l, addig a QS a minta ut�ni karakter alapj�n. Teh�t elroml�s eset�n:
� Ha a minta ut�ni karakter benne van a mint�ban, akkor jobbr�l az els� el�fordul�s�ra illesztj�k


3. �braQS - eltol�s ha a minta ut�ni karakter benne van a mint�ban � Ha a minta ut�ni karakter nincs benne a mint�ban, akkor a mint�t ezen karakter ut�n illesztj�k.

4. �bra: QS - eltol�s ha a minta ut�ni karakter nincs benne a mint�ban
Az eltol�s kisz�m�t�s�t megint el� lehet seg�teni egy t�mbbel, most azonban, mivel nem a minta az
��rdekes, �es nem tudjuk pontosan mely karakterek szerepelnek a sz�vegben, �gy a t�mbbe az eg�sz abc-t fel kell venn�nk (7. ��bra)

5. �bra: QS - Az eltol�st el�seg�t� t�mb (Shift[0a0...0z0]) konstru�l�sa

K�t indexet k �es j futtatunk a sz�vegen illetve a mint�n.
Ha a sz�veg k + j-edik eleme megegyezik a minta j-edik karakter�vel, akkor l�ptetj�k j-t (mivel a sz�vegben k + j-edik elemet n�zz�k, ��gy el�g j-t n�velni).
Ha nem egyeznek meg, akkor:
Ha a minta m�r a sz�veg v�g�n van (k = n?m), akkor csak n�velj�k k-t eggyel, ami hamiss� teszi a ciklus felt�telt.
Ha m�g nem vagyunk a sz�veg v�g�n k-t toljuk annyival, amennyivel lehet (ezt az el�re be�ll�tott Shift t�mb hat�rozza meg). Es a� j-t vissza�ll�tjuk 1-re.
Addig megy�nk, m�g vagy a minta v�g�re ��r�nk j-vel, vagy a mint�t tov�bb toltuk a sz�veg v�g�n�l.
El�bbi esetben egyez�st tal�ltunk, m�g az ut�bbiban nem.

6. �bra QS
RK algoritmus
A Rabin-Karp algoritmus l�nyege, hogy minden bet�h�z az �ab�c�b�l egy sz�mjegyet rendel�nk, �es a keres�st sz�mok �sszehasonl�t�s�val v�gezz�k. Vil�gos, hogy ehhez egy ��b�c� m�retnek megfelel� sz�mrendszerre lesz sz�ks�g�nk. A sz�vegb�l mindig a minta hossz�val egyez� r�szeket szel�nk ki, �es ezeket hasonl�tjuk ��ssze.
P�lda:
Minta: BBAC ? 1102
Sz�veg: DACABBAC ? 30201102, amib�l a k�vetkez� sz�mokat ��ll�tjuk el�: 3020, 0201, 2011, 0110, 1102
A fent l�that� szeletek lesznek az si-k.
Az algoritmus m�k�d�s�hez azonban sz�mos apr� ��tletet alkalmazunk:
A minta sz�mokk� alak�t�s�t Horner-m�dszer seg�ts�g�vel v�gezz�k.

7. �bra: RK - Horner-m�dszer
Az ord() f�ggv�ny az egyes bet�knek megfelel� sz�mot adja vissza. A d a sz�mrendszer alapsz�ma.
A sz�veg mint�val megegyez� hossz�� szelet�inek (si) el��ll�t�sa: s0-t a Horner-m�dszerrel ki tudjuk sz�molnia. Ezek ut�n si+1 a k�vetkez�k�pp sz�moland�: si+1 = (si ? ord(S[i]) � dm?1) � d + ord(S[i + 1]) Felmer�lhet a k�rd�s, hogy az ilyen magas alapsz�m�� sz�mrendszerek nem okoznak-e gondot az �br�zol�sn�l? A k�rd�s jogos. Vegy�k a k�vetkez� ��letszer�� p�ld�t: 4 b�jton ��br�zoljuk a sz�mainkat (232). Az abc legyen 32 elem�� (d = 32), a minta 8 hossz�� (m = 8). Ekkor a dm?1 kisz�m�t�sa: 327 = (25)7 = 235 , ami m�r nem ��br�zolhat� 4 b�jton. Ennek kik�sz�b�l�s�re vezess�nk be egy nagy p pr�met, melyre d � p m�g ��br�zolhat�. Es a� m�veleteket sz�moljuk mod p. Ekkor term�szetesen a kongruencia miatt lesz olyan eset, amikor az algoritmus egyez�st mutat, mikor val�j�ban nincs. Ez nem okoz gondot, mivel ilyen esetben karakterenk�nti egyez�st vizsg�lva ezt a probl�m�t kezelni tudjuk. (Ford�tott eset nem fordul el� teh�t nem lesz olyan eset, mikor karakterenk�nti egyez�s van, de numerikus nincs). [Ha p kell�en nagy, a jelens�g nagyon ritk�n fordul el�.]
A mod p sz�m�t�s egy m�sik probl�m�t is felvet. Ugyanis a kivon�s alkalm�val negat�v sz�mokat is kaphatunk.
P�ld�ul: Legyen p = 7, ekkor, ha ord(S[i]) = 9, akkor el�z� sz�m�t�s ut�n si = 2..., de ebb�l ord(S[i]) � dm?1 = 9 � 103 = 9000-et vonunk ki negat�v sz�mot kapunk. Megold�sk�nt si+1-et k�t lep�sben sz�moljuk:
	s := (si + d � p ? ord(S[i]) � dm?1)	mod p
	si+1 := (s � d + ord(S[i + 1]))	mod p
A fentiek alapj�n az algoritmus a k�vetkez� (ld. 8. ��bra)
* Kisz�moljuk dm?1-et (dm1)
* Egy iter�ci�ban meghat�rozzuk Horner-m�dszerrel a minta sz�mait (x) �es s0-t
* Ellen�rizz�k, hogy egyeznek-e
* Addig sz�molgatjuk si ��rteket m�g a minta nem egyezik si-vel, vagy a minta a sz�veg v�g�re nem �rt.
8. �bra: RK



GENERATE.H
A generate.h fej�llom�ny 3 p�rhuzamos�tott met�dust tartalmaz, melyek teljes m�rt�kben a saj�t �ltalam kital�lt �s lefejlesztett algoritmusokat tartalmazz�k. Tov�bba egy szekvenci�lis kiirat� met�dust, a gener�l�st megval�s�t� generate f�ggv�nyt, �s a szok�sos �r�n haszn�lt cuda error ellen�rz� met�dust.
GENERATE_KERNEL
A met�dus param�ter�l v�rja a k�t m�g �res szekvenci�t, azok m�ret�t, n illetve egy curand gener�lt unsigned int t�mb�t mely a genomok eloszl�s�t fogja megadni. A genom t�mb tartalmazza a 4 karaktert amit egy genom tartalmazhat. Az els� szekvenci�ba ker�lnek a random kapott genom t�mb elemek, a m�sodik szekvenci�ba ker�lnek a random kapott elem ellent�tei egy switch-case seg�ts�g�vel. Ezek ut�n a met�dus egy random helyen elrejti a mut�ci�t, a param�ter�l kapott random t�mb alapj�n ami nem mellesleg az implement�ci� szerint Poisson eloszl�s�.
HIDE(KERNEL)
A hide met�dus v�gzi a pattern elrejt�s�t a szekvenci�k valamelyik�ben, ehhez param�ter�l v�rja k�t random sz�mot, hogy melyik szekvenci�ban �s melyik karaktern�l(hanyadik elemn�l) kezdje el. innent�l a met�dus azonos elven m�k�dik mint a generate f�ggv�ny, vagyis switch � case-ek seg�ts�g�vel m�dos�tja egyszerre a k�t t�mb�t.
PRINT_DNA_KERNEL
Egy egyszer� p�rhuzamos�tott t�mb kiirat� met�dus, melyhez a l�p�seket stride-ok seg�ts�g�vel implement�ltam.
GENERATE MET�DUS
A generate met�dus v�gzi a mem�ria foglal�sokat a GPU r�sz�re, a hide kernelhez sz�ks�ges random sz�mok, illetve a curand poisson t�mb gener�l�s�t. A sz�ks�ges mem�ria m�sol�sok ut�n futtatja a 3 fent eml�tett met�dus futtat�s�t �s szinkroniz�l�s�t.


GPU_SEARCH.H
Mint ahogy a �A megval�s�tott projekt� fejezetben m�r le�rtam, nem siker�lt a keres�sek p�rhuzamos�t�sa, a header f�jl fordul, de a keres�s nem tal�l eredm�nyt a szekvenci�ban. Az algoritmusok p�rhuzamos�t�s�hoz k�t a github-on fellelhet� implement�ci�t vettem alapul, de �t�r�suk nem �rte el a kell� sikert. A KMP algoritmushoz, a https://github.com/Fang-Haoshu/KMP-on-CUDA oldalon tal�lhat� f�jlokat k�rtem seg�ts�g�l. M�g az RK algoritmushoz a https://github.com/mstftrn/rabin-karp-for-CUDA rep�t haszn�ltam fel.
CPU_SEARCH
A CPU_seearch fej�llom�ny tartalmazza a CPU keres�sekhez sz�ks�ges algoritmusokat, melyek megfelelnek az algoritmusok el�z�ekben megadott le�r�s�nak. A gyors halad�s �rdek�ben, �s mert a Tan�r �r is javasolta, a CPU algoritmusokat, m�r k�sz implement�ci�k alapj�n k�sz�tettem.
A KMP algoritmust
https://www.techiedelight.com/implementation-kmp-algorithm-c-cpp-java/,
A QS algoritmust
https://www.geeksforgeeks.org/boyer-moore-algorithm-good-suffix-heuristic/
Az RK algoritmust
https://www.geeksforgeeks.org/rabin-karp-algorithm-for-pattern-searching/
alapj�n implement�ltam a feladathoz.
FORD�T�S �S FUTTAT�S
FORDT�S
A programot a -lcurand kapcsol�val kell ford�tani a generate.h �llom�nyban tal�lhat� curand random gener�tor miatt. A ford�t�s teljes parancsa: nvcc DNA_seq.cu -lcurand -o dna
FUTTAT�S
Innent�l a szok�sos m�don futtathat� az �llom�ny, viszont az �tl�that�s�g �rdek�ben �rdemes egy txt f�jlba futtatni, pontosabban: ./dna > result.txt
MEGJEGYZ�S
A DNA_GPU.cu nev� f�jllal futtathat�ak az algoritmusok futtat�s�ra tett pr�b�lkoz�saim.
A CPU_search.h �s a GPU_search.h egy�tt nem include-olhat�, ez�rt ker�lt k�l�n f�jlba.
A helyes parancs: nvcc DNA_GPU.cu -lcurand -o dnagpu.
A futtat�s az el�z�ekkel megegyez�.
A szekvenci�k m�rete n�velhet�, de a gyors futtat�s v�gett, nem aj�nlott(A CPU keres�sek nagy m�ret� t�mb�n lass�ak.) Tov�bb� az elrejt�st hozz� kell igaz�tani a t�mbh�z:
int j = (i-1)%PATTERN_SIZE; // if the sequence size is 15000 i+2


