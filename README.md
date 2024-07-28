## Introducció

Benvingut al projecte de l'Exercici Lliurable 4 de Laboratori d'IDI 2023/2024 Q2. Aquest projecte està dissenyat per proporcionar experiència pràctica amb Qt 5 i OpenGL, on visualitzaràs i manipularàs un circuit de carreres circular amb cotxes en moviment. L'objectiu principal és implementar i millorar el codi esquelètic proporcionat per aconseguir la funcionalitat i els efectes visuals desitjats segons les especificacions de la tasca universitària.

## Guia d'Instal·lació

### Prerequisits

Abans de començar, assegura't de tenir instal·lats els següents elements al teu sistema:

- **Qt 5**: Es recomana Qt 5.15 o posterior.
- **OpenGL**: Una versió compatible d'OpenGL.
- **Compilador C++**: Un compilador que suporti C++11 o posterior.
- **CMake**: Per construir el projecte.

### Passos per Instal·lar i Configurar

1. **Descarrega i Instal·la Qt 5**:
    - Ves a la [pàgina web de Qt](https://www.qt.io/download).
    - Descarrega l'Instal·lador en Línia de Qt.
    - Executa l'instal·lador i segueix les instruccions per instal·lar Qt 5.x

2. **Assegura't que OpenGL està Instal·lat**:
    - OpenGL hauria d'estar disponible en la majoria de sistemes per defecte. Pots verificar la seva presència executant:
      ```sh
      glxinfo | grep "OpenGL version"
      ```
    - Si OpenGL no està instal·lat, consulta les instruccions específiques del teu sistema operatiu per instal·lar-lo.

3. **Descarrega el Repositori del Projecte**:
    - Clona el repositori:
      ```sh
      git clone https://github.com/mpenalverguilera/idi-exercici-4-lab-2324q2.git
      ```

4. **Construeix el Projecte**:
    - Navega fins al directori del projecte:
      ```sh
      cd /path/to/your/directory/idi-exercici-4-lab-2324q2
      ```
    - Executa QMake per crear el Makefile (només cal fer-ho un cop):
      ```sh
      qmake
      ```
    - Construeix el projecte utilitzant `make`:
      ```sh
      make
      ```

5. **Executa l'Aplicació**:
    - Després d'una construcció exitosa, pots executar l'aplicació:
      ```sh
      ./Exercici-4
      ```

## Guia de Controls

L'aplicació inclou diversos controls interactius per manipular l'escena i animar els cotxes. Aquí tens una guia dels controls:

### Controls de la Càmera

- **Interacció amb el Ratolí**: 
  - Rota la càmera fent clic i arrossegant el ratolí.

### Controls del Teclat

- **Fletxa Amunt (`Key Up`)**:
  - Mou els dos cotxes cap endavant al voltant del circuit. Els cotxes giraran al voltant de l'eix Y en 2° i 3° respectivament.
  
- **Activar/Desactivar Llum de la Càmera (`L`)**:
  - Encén i apaga la llum de la càmera. Quan la llum de la càmera està apagada, el color de fons canvia a gris fosc (0.3, 0.3, 0.3).
  
- **Activar/Desactivar Temporitzador d'Animació (`S`)**:
  - Comença o atura el QTimer que fa que els cotxes es moguin contínuament. El temporitzador s'activa cada 50 mil·lisegons, fent que els cotxes es moguin al voltant de la pista.


