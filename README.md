Acest proiect reprezintă o simulare interactivă a unui sistem feroviar complex, dezvoltat în C++ utilizând API-ul OpenGL. Accentul cade pe modelarea ierarhică a obiectelor, animația bazată pe noduri și un sistem de cameră custom.
<img width="2000" height="1000" alt="Screenshot 2026-03-02 101728" src="https://github.com/user-attachments/assets/a18b2f66-efa5-497e-8065-d0764eaa7647" />
<img width="2000" height="937" alt="Screenshot 2026-03-02 103139" src="https://github.com/user-attachments/assets/e6fd4eba-997c-4afc-ab83-092ddb5f3b4a" />

## Funcționalități Principale

### Sistem Cameră Avansat
Sistemul de vizualizare (`lab_camera.h`) suportă moduri de navigare dinamice:
* **First-Person Mode**: Permite rotația liberă a vectorilor `forward`, `up` și `right`.
* **Third-Person Mode**: Camera orbitează în jurul trenului la o distanță predefinită.
* **Dual Projection**: Comutare în timp real între **Proiecție Perspectivă** (pentru realism) și **Proiecție Ortografică** (pentru vizualizare tehnică de sus).
* **Dynamic FOV**: Controlul câmpului vizual (Zoom) la runtime.

### Modelare Ierarhică și Animație
* **Construcție din Primitive**: Locomotiva și vagoanele sunt asamblate din cuburi, cilindri și piramide generate procedural.
* **Mișcare pe Graf**: Trenul parcurge un traseu definit de structuri de tip nod, utilizând interpolare liniară.
* **Conectori Dinamici**: Barele dintre vagoane își recalculează poziția și rotația în fiecare cadru pentru a menține integritatea vizuală a garniturii.

### Mediu Procedural și Intersecții
* **Adaptare la Teren**: Sistemul generează automat **poduri** peste apă, **tuneluri** prin munți și **șine standard** pe câmpie.
* **Logica Intersecțiilor**: Utilizatorul poate preselecta direcția de deplasare, trenul alegând automat ramificația corespunzătoare la atingerea nodului de tip intersecție.

* ## Controale

| Tasta | Acțiune |
| :--- | :--- |
| **W, A, S, D** | Mișcare Cameră / Selectare direcție tren în intersecții |
| **Q, E** | Translație cameră pe axa verticală (Sus/Jos) |
| **Mouse Right Click** | Activare mod rotire cameră (First Person) |
| **CTRL + Mouse RC** | Rotire Third-Person (în jurul trenului) |
| **O / P** | Comutare mod Ortografic / Perspectivă |
| **L / M** | Zoom In / Zoom Out (Modificare FOV) |
