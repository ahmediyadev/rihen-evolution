#include <iostream>
#include <cstring>
using namespace std;

#define MAX_DEVICES 100
#define MAX_CONNECTIONS 20

// Classe représentant un appareil dans le graphe
class Device {
public:
    char name[30];                        // Nom de l'appareil
    char connections[MAX_CONNECTIONS][30]; // Liste des connexions (noms des autres appareils)
    int connectionCount;                 // Nombre de connexions

    Device() {
        name[0] = '\0';
        connectionCount = 0;
    }

    void setName(const char* deviceName) {
        strcpy(name, deviceName);
    }

    bool isConnectedTo(const char* other) {
        for (int i = 0; i < connectionCount; ++i) {
            if (strcmp(connections[i], other) == 0)
                return true;
        }
        return false;
    }

    void addConnection(const char* other) {
        if (!isConnectedTo(other) && connectionCount < MAX_CONNECTIONS) {
            strcpy(connections[connectionCount], other);
            connectionCount++;
        }
    }
};

// Classe représentant le graphe de réseau
class NetworkGraph {
private:
    Device devices[MAX_DEVICES];  // Liste de tous les appareils du réseau
    int deviceCount;              // Nombre total d'appareils

    // Trouver l’index d’un appareil par son nom
    int findDeviceIndex(const char* name) {
        for (int i = 0; i < deviceCount; ++i) {
            if (strcmp(devices[i].name, name) == 0)
                return i;
        }
        return -1; // Pas trouvé
    }

public:
    NetworkGraph() {
        deviceCount = 0;
    }

    // Ajouter un appareil au réseau
    bool addDevice(const char* name) {
        if (deviceCount >= MAX_DEVICES) {
            cout << "Erreur : nombre maximum d'appareils atteint.\n";
            return false;
        }

        if (findDeviceIndex(name) != -1) {
            cout << "Erreur : l'appareil existe déjà.\n";
            return false;
        }

        devices[deviceCount].setName(name);
        deviceCount++;
        return true;
    }

    // Ajouter une connexion entre deux appareils
    bool addConnection(const char* name1, const char* name2) {
        int index1 = findDeviceIndex(name1);
        int index2 = findDeviceIndex(name2);

        if (index1 == -1 || index2 == -1) {
            cout << "Erreur : l'un des appareils n'existe pas.\n";
            return false;
        }

        devices[index1].addConnection(name2);
        devices[index2].addConnection(name1);
        return true;
    }

    // Afficher les connexions d’un appareil
    void displayConnections(const char* name) {
        int index = findDeviceIndex(name);
        if (index == -1) {
            cout << "Erreur : appareil introuvable.\n";
            return;
        }

        cout << "Connexions de l'appareil " << name << " :\n";
        if (devices[index].connectionCount == 0) {
            cout << "   Aucun.\n";
            return;
        }

        for (int i = 0; i < devices[index].connectionCount; ++i) {
            cout << "   - " << devices[index].connections[i] << "\n";
        }
    }

    // Afficher tout le réseau
    void displayAll() {
        if (deviceCount == 0) {
            cout << "Le graphe est vide.\n";
            return;
        }

        for (int i = 0; i < deviceCount; ++i) {
            displayConnections(devices[i].name);
        }
    }

    // Recherche en largeur (BFS) pour trouver un appareil
    void bfsSearch(const char* startName) {
        bool visited[MAX_DEVICES] = { false };
        int queue[MAX_DEVICES];
        int front = 0, rear = 0;

        int startIndex = findDeviceIndex(startName);
        if (startIndex == -1) {
            cout << "Erreur : appareil de départ introuvable.\n";
            return;
        }

        queue[rear++] = startIndex;
        visited[startIndex] = true;

        cout << "Parcours BFS depuis l'appareil " << startName << " :\n";

        while (front < rear) {
            int currentIndex = queue[front++];
            cout << " - " << devices[currentIndex].name << "\n";

            for (int i = 0; i < devices[currentIndex].connectionCount; ++i) {
                int neighborIndex = findDeviceIndex(devices[currentIndex].connections[i]);
                if (!visited[neighborIndex]) {
                    visited[neighborIndex] = true;
                    queue[rear++] = neighborIndex;
                }
            }
        }
    }
};

// Programme principal
int main() {
    NetworkGraph net;

    // Test : ajouter 4 appareils
    net.addDevice("A");
    net.addDevice("B");
    net.addDevice("C");
    net.addDevice("D");

    // Test : ajouter 3 connexions
    net.addConnection("A", "B");
    net.addConnection("A", "C");
    net.addConnection("B", "D");

    // Affichage
    net.displayConnections("A");
    net.displayConnections("C");
    net.displayConnections("D");

    // Test : erreur (connexion avec appareil inexistant)
    net.addConnection("A", "Z");

    // Recherche BFS
    net.bfsSearch("A");

    // Test : graphe vide
    NetworkGraph empty;
    empty.displayAll();

    return 0;
}
