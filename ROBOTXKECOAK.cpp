// 16520063 / PUTRI NURHALIZA
// 16920188 / FADIL ALDITO PUTRA
// 16420189 / BAGAS SHALAHUDDIN WAHID


#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;


//pergerakan dan current position robot
auto Move(int *xPtr, int *yPtr, int dist_minX, int dist_minY){
    struct resMove {int x; int y;};

    int move;
    printf ("Pilihan bergerak 1.Kanan, 2.Kiri, 3.Maju, 4.Mundur : ");
    scanf ("%d", &move);
    switch (move) {
        case 1:
            if (dist_minX == 1 && dist_minY == 0) {
                printf ("Tidak bisa menabrak, ada Kecoak di kanan\n"); break;
            }
            else {
                *xPtr += 1;break;
            }
        case 2:
            if (dist_minX == (-1) && dist_minY == 0) {
                printf ("Tidak bisa menabrak, ada Kecoak di kiri\n"); break;
            }
            else if (*xPtr <= 0) {
                printf ("Tidak dapat bergerak ke kiri, area di luar map\n"); break;
            }
            else {*xPtr -= 1; break;
            }
        case 3:
            if (dist_minX == 0 && dist_minY == 1) {
                printf ("Tidak bisa menabrak, ada Kecoak di depan\n"); break;
            }
            else {*yPtr += 1; break;
            }
        case 4:
            if (dist_minX == 0 && dist_minY == (-1)) {
                printf ("Tidak bisa menabrak, ada Kecoak di belakang\n"); break;
            }
            else if (*yPtr <= 0 ) {
                printf ("Tidak dapat bergerak ke belakang, area di luar map\n"); break;
            }
            else {*yPtr -= 1; break;
            }
        default: break;
    }
    int x = *xPtr;
    int y = *yPtr;
    return resMove {x, y};
}

// jarak robot dengan setiap kecoak
auto Distance(int *xPtr, int *yPtr, vector<vector<int>> posEnemy, int botHealth, int enmyDmgRange){
    struct resDist {int dist_minX; int dist_minY; double dist_min; int pos_minX; int pos_minY; int botHealth;};

    double dist_min = 100;
    int dist_minX = 100;
    int dist_minY = 100;
    int pos_minX;
    int pos_minY;
    int nDmgEnemy = 0;

    for (int i = 0; i < posEnemy.size(); i++){
        int distX = posEnemy[i][0] - *xPtr;
        int distY = posEnemy[i][1] - *yPtr;
        double dist = sqrt(pow(distX,2) + pow(distY,2));
        if (dist < dist_min){       // mencari posisi dan jarak kecoak terdekat dari robot
            dist_min = dist;   
            dist_minX = distX;
            dist_minY = distY;
            pos_minX  = posEnemy[i][0];
            pos_minY = posEnemy[i][1];
        }
        if (dist <= enmyDmgRange){  // menentukan robot terkena damage dari berapa banyak kecoak
            nDmgEnemy += 1;
        }
    }
    
    botHealth -= nDmgEnemy*100;
    return resDist {dist_minX, dist_minY, dist_min, pos_minX, pos_minY, botHealth};
}

auto Attack (vector<vector<int>> posEnemy, double dist_min, int pos_minX, int pos_minY, int dead_Enemy, int botDamage, int botDmgRange){
    struct resAttack {vector<vector<int>>posEnemy; int dead_Enemy;};
    
    if (dist_min <= botDmgRange){
        for (int i = 0; i < posEnemy.size(); i++){
            if (posEnemy[i][0] == pos_minX && posEnemy[i][1] == pos_minY){
                posEnemy[i][2] -= botDamage;
            }
        }
    }
    else {printf ("Tidak dapat menembak kecoak karena di luar jangkauan\n");}
    
    for (int i = 0; i < posEnemy.size(); i++){
        if (posEnemy[i][2] == 0){
            posEnemy.erase (posEnemy.begin()+i);
            printf("SATU KECOAK BERHASIL DIBUNUH\n");
            dead_Enemy += 1;
        }
    }

    return resAttack {posEnemy, dead_Enemy};
}

int main(){

    printf ("----------- Urang Robot Orang X Kecoak Mutan -----------\n\n");

    //posisi awal robot
    int x = 0;
    int y = 0;
    int *xPtr = &x;
    int *yPtr = &y;

    //atribut robot
    int botHealth = 5000;
    int botDamage = 500;
    int botDmgRange = 3;

    // posisi awal dan atribut Kecoak
    int nEnemy = 5;
    int dead_Enemy = 0;
    int enmyDamage = 500;
    int enmyDmgRange = 3;
    srand(time(0));
    vector<vector<int>> posEnemy(nEnemy);
    for (int i = 0; i < posEnemy.size(); i++){
        posEnemy[i].resize(3);
        posEnemy[i][0] = (rand() % 20) + 1;    // absis posisi kecoak
        posEnemy[i][1] = (rand() % 20) + 1;    // ordinat posisi Y kecoak
        posEnemy[i][2] = 1000;                 // atribut health kecoak
    }


    while(true) {
        printf ("Posisi robot = (%d, %d)\n", *xPtr, *yPtr);
        
        /* JIKA INGIN MENGECEK POSISI DAN HEALTH TIAP KECOAK
        for (int i = 0; i < posEnemy.size(); i++){
            printf("Kecoak (%d, %d) health = %d\n", posEnemy[i][0], posEnemy[i][1], posEnemy[i][2]);
        }
        */
        
        auto resDist = Distance(xPtr, yPtr, posEnemy, botHealth, enmyDmgRange);
        printf ("Posisi kecoak terdekat terhadap robot = <%d, %d>\n", resDist.dist_minX, resDist.dist_minY);
        printf ("Jarak kecoak terdekat dari robot = %.2f\n", resDist.dist_min);
        
        botHealth = resDist.botHealth;
        printf ("Health robot = %d\n", botHealth);
        if (botHealth == 0){
            printf ("\nProgram berhenti karena robot kehabisan health.\n");
            printf ("Sebanyak %d kecoak berhasil dibunuh.\n", dead_Enemy);
            break;
        }

        if (posEnemy.size() == 0){
            printf ("CONGRATULATION!! Robot berhasil membunuh seluruh kecoak.\n Total terdapat 20 kecoak.\n");
            break;
        }

        // pilihan yang akan dilakukan robot
        printf ("\nOpsi 1.Bergerak 2.Menembak 3.Berhenti : ");
        int opsi;
        scanf ("%d", &opsi);
        if (opsi == 1) {
            auto resMove = Move(xPtr, yPtr, resDist.dist_minX, resDist.dist_minY);
            *xPtr = resMove.x;
            *yPtr = resMove.y;
        }
        else if (opsi == 2){
            auto resAttack = Attack (posEnemy, resDist.dist_min, resDist.pos_minX, resDist.pos_minY, dead_Enemy, botDamage, botDmgRange);
            posEnemy = resAttack.posEnemy;
            dead_Enemy = resAttack.dead_Enemy;
        }
        else if (opsi == 3){
            printf("\nSebanyak %d kecoak berhasil dibunuh.\n", dead_Enemy);
            printf("Program selesai.");
            break;
        }
        else {break;}
    
    }
    return 0;

}