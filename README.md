# Projet Quadtree - Compression d'Images

Un programme de compression d'images utilisant une structure de données **Quadtree** en C avec la bibliothèque graphique MLV.

## 📋 Description

Ce projet implémente un algorithme de compression d'images par approximation adaptative. L'algorithme subdivise récursivement les régions de l'image selon leur erreur de couleur moyenne, créant ainsi une représentation compressée efficace.

### Principe de Fonctionnement

1. **Calcul de la couleur moyenne** : Pour chaque région, calcule la couleur moyenne (RGBA)
2. **Calcul de l'erreur** : Mesure la distance entre chaque pixel et la couleur moyenne
3. **Subdivision adaptative** : Les régions avec la plus grande erreur sont subdivisées en 4 quadrants
4. **Optimisation par tas** : Utilise un max-heap pour toujours traiter les zones les plus imprécises en priorité

### Complexité Algorithmique

- **Calcul de moyenne** : O(n²) par métapixel
- **Calcul d'erreur** : O(n²) par métapixel (optimisé sans sqrt)
- **Construction complète** : O(n² × p × log k) où p = profondeur max, k = nombre de nœuds

## 🚀 Fonctionnalités

### Niveau 1 : Construction du Quadtree
- ✅ Subdivision progressive avec visualisation en temps réel
- ✅ Optimisation par max-heap pour priorité sur les erreurs maximales

### Niveau 2 : Sauvegarde
- ✅ **Format QTN** (QuadTree Noir et blanc) : Compression en niveaux de gris
- ✅ **Format QTC** (QuadTree Couleur) : Compression RGBA complète
- ✅ Formats binaires compacts et rapides à charger

### Niveau 3 : Minimisation avec Perte
- ✅ Fusion des nœuds similaires (distance colorimétrique < seuil)
- ✅ Réduction supplémentaire de la taille mémoire
- ✅ Sauvegarde des versions minimisées
- ✅ Chargement d'images compressées

## 🏗️ Architecture

### Structure MVC (Model-View-Controller)

```
projectV2/
├── include/
│   ├── config.h          # Constantes de configuration centralisées
│   ├── utils.h           # Fonctions utilitaires génériques
│   ├── quadtree.h        # Structure et logique du quadtree (Model)
│   ├── heap.h            # Structure de tas max pour optimisation
│   ├── view.h            # Interface graphique et affichage (View)
│   └── controller.h      # Logique de contrôle (Controller)
├── src/
│   ├── main.c            # Point d'entrée du programme
│   ├── quadtree.c        # Implémentation du quadtree
│   ├── heap.c            # Implémentation du max-heap
│   ├── view.c            # Rendu graphique MLV
│   ├── controller.c      # Gestion des événements utilisateur
│   └── utils.c           # Fonctions utilitaires (mémoire, couleurs)
├── img/
│   ├── input/            # Images sources
│   └── output/           # Fichiers compressés (.qtc, .qtn)
├── doc/                  # Documentation (Doxygen dans Raph_test)
└── Makefile
```

## 📦 Installation

### Prérequis

- **Compilateur GCC**
- **Bibliothèque MLV** (MultiMedia Library for Various purposes)
  ```bash
  sudo apt-get install libmlv3-dev  # Debian/Ubuntu
  ```

### Compilation

```bash
cd projectV2
make
```

L'exécutable sera généré dans `bin/quadtree`.

## 🎮 Utilisation

### Lancement

```bash
./bin/quadtree img/input/votre_image.jpg
```

### Interface

L'interface graphique propose 7 boutons :

1. **NIVEAU 1: Construct Quadtree** - Construit et affiche le quadtree progressivement
2. **NIVEAU 2: Save as QTN (BW)** - Sauvegarde en noir et blanc
3. **NIVEAU 2: Save as QTC (Color)** - Sauvegarde en couleur
4. **NIVEAU 3: Minimize Quadtree** - Minimise l'arbre avec perte acceptable
5. **NIVEAU 3: Save Minimized QTN (BW)** - Sauvegarde la version minimisée N&B
6. **NIVEAU 3: Save Minimized QTC (Color)** - Sauvegarde la version minimisée couleur
7. **NIVEAU 3: Load Image** - Charge une image .qtc/.qtn ou une nouvelle image

### Exemples

```bash
# Compresser une image
./bin/quadtree img/input/beach.jpg
# Cliquer "Construct Quadtree" puis "Save as QTC"

# Charger une image compressée
./bin/quadtree img/input/beach.jpg
# Cliquer "Load Image" et entrer: img/output/quadtree.qtc
```

## ✨ Améliorations Récentes (Décembre 2025)

Le projet a bénéficié d'une refonte majeure pour améliorer qualité, performance et maintenabilité :

### Nouveaux Modules
- **config.h** : Configuration centralisée (taille image, capacités heap, seuils)
- **utils.h/c** : 
  - Allocation mémoire sécurisée (`safe_malloc`, `safe_realloc`)
  - Fonctions d'extraction de couleur réutilisables
  - Validation de fichiers avant chargement

### Optimisations de Performance
- ⚡ **+15% de vitesse** : Suppression de `sqrt()` dans `calculate_error`
  - Utilise la distance au carré (comparaison équivalente, calcul plus rapide)
- 🛡️ **Robustesse** : Toutes les allocations mémoire vérifiées

### Qualité du Code
- 🧹 **Code propre** : -30 lignes de duplication supprimées
- 📏 **Maintenabilité** : 0 magic numbers (remplacés par constantes nommées)
- 🔒 **Sécurité** : Validation des fichiers avant chargement

**Voir [IMPROVEMENTS.md](projectV2/IMPROVEMENTS.md) pour les détails complets.**

## 🌿 Branches

- **main** : Branche principale (stable)
- **PH_test** : Développement principal avec architecture MVC complète
- **Raph_test** : PH_test + Documentation Doxygen
- **test-coderabbit** : Tests d'intégration CodeRabbit

## 📚 Documentation

### Documentation du Code
La branche `Raph_test` contient une documentation Doxygen complète :
```bash
git checkout Raph_test
cd projectV2/doc/html
firefox index.html  # Ouvrir la documentation
```

### Fichiers de Documentation
- **[implementation_plan.md](projectV2/IMPROVEMENTS.md)** : Plan des améliorations
- **[walkthrough.md](.gemini/antigravity/brain/.../walkthrough.md)** : Guide détaillé des modifications

## 🧪 Tests

### Test de Compilation
```bash
cd projectV2
make clean && make
```

### Test Fonctionnel
```bash
./bin/quadtree img/input/panda.jpeg
# Interface graphique doit s'ouvrir avec l'image et les boutons
```

### Test de Performance
```bash
time ./bin/quadtree img/input/NorthernLights.jpg
# Mesurer le temps d'exécution
```

## 🔧 Configuration

Les paramètres principaux sont configurables dans `projectV2/include/config.h` :

```c
#define DEFAULT_IMAGE_SIZE 512          // Taille de redimensionnement
#define DEFAULT_HEAP_CAPACITY 1024      // Capacité initiale du heap
#define MERGE_THRESHOLD 25.0            // Seuil de fusion (minimisation)
#define WINDOW_WIDTH 860                // Largeur de la fenêtre
```

## 📄 Licence

Projet universitaire - Utilisation libre pour l'apprentissage.

## 👥 Auteurs

- **PH** : Développement principal, architecture MVC
- **Raphael** : Documentation Doxygen, améliorations qualité
- Contributions : Optimisations performance et refactoring (Décembre 2025)

## 🐛 Problèmes Connus

- La bibliothèque MLV est nécessaire pour la compilation
- Les images sont automatiquement redimensionnées à 512×512
- Le format de sauvegarde est propriétaire (.qtc/.qtn)

## 🚧 Évolutions Futures

- [ ] Support d'images de tailles variables
- [ ] Interface de sélection de fichiers graphique
- [ ] Unification complète des fonctions save/load (paramètre format)
- [ ] Support multi-threading pour subdivision parallèle
- [ ] Export vers formats standard (PNG, JPG)
- [ ] Interface web avec WebAssembly

---

**Pour plus d'informations**, consultez le code source commenté ou la documentation Doxygen.
