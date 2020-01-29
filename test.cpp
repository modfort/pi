#include <iostream>
#include <gmpxx.h> //On inclut la bibliothèque gmp
#include <thread>
#include <mutex>

//variables globale utilisé par les threads qui contient la somme de chacun de leur calcul
mpf_t somme;

void pi(int precsion,int debut, int fin,std::mutex & lock)
{   //declaration des varibales
    mpf_t puissance_16,div_16 , premier , deuxieme , troisieme , quatrieme , mult_8,somme_int ;
    //initialisation des variables
    mpf_init2(puissance_16,precsion*4);
    mpf_set_ui(puissance_16,16);
    mpf_pow_ui(puissance_16,puissance_16,debut);
    mpf_init2(div_16,precsion*4);
    mpf_init2(premier,precsion*4);
    mpf_init2(deuxieme,precsion*4);
    mpf_init2(troisieme,precsion*4);
    mpf_init2(quatrieme,precsion*4);
    mpf_init2(somme_int,precsion*4);
    mpf_init2(mult_8,precsion*4);
    mpf_set_ui(mult_8,8);
    //boucle de calcul  
    // je detaille pas a pas ce que je fait
	for(int i = debut ; i < fin ; i++)
	{    
		//Cette fonction attribue à "a" le premier nombre premier après "a"
        mpf_ui_div(div_16 , 1 , puissance_16 );//1/16
        mpf_mul_ui(mult_8 , mult_8 , i);//8*i
        mpf_add_ui(mult_8  , mult_8 , 1 );//mult_8*i+1
        mpf_ui_div(premier , 4 , mult_8);//4/mult_8
        mpf_add_ui(mult_8 , mult_8 , 3);//div8+3
        mpf_ui_div(deuxieme , 2 , mult_8);//2/mult_8
        mpf_add_ui(mult_8 , mult_8 , 1);//div8+1
        mpf_ui_div(troisieme , 1 , mult_8);//1/mult_8
        mpf_add_ui(mult_8 , mult_8 ,1);//div8+1
        mpf_ui_div(quatrieme , 1 , mult_8);//1/diV8
        // dans chauqe soustraction j'effectue un calcul de 4/8*i -(2/8*i+4 + 1/8*i+5 + 1/8*i+-)
        mpf_sub(premier , premier ,deuxieme);
        mpf_sub(premier , premier ,troisieme);
        mpf_sub(premier , premier , quatrieme);
        
        mpf_mul(premier , div_16 , premier);//1/16^k * la somme caculer precedemment
        mpf_add(somme_int , premier , somme_int);// je l'ajoute a la variable temporaire creer 
        mpf_mul_ui (puissance_16, puissance_16, 16 );//16^i au lieu de faire a chaque fois un calul de 16^k on le mutliplie a chaque fois par 16
        mpf_set_ui(mult_8,8);    //on remet Mul 8 a 8 a chaque tour de boucle 
    }

 
    lock.lock();
    mpf_add(somme,somme,somme_int);//on additionne la somme calculer localement avec a globale
    lock.unlock();
}

int main(int argc, char  *argv[])
{   
//on verifie le nombre d'arguments
    if(!argv[1]  || atoi(argv[1])<=0 ||!argv[2]  || atoi(argv[2])<=0  ){
        fprintf(stderr, "%s\n", "erreur de parametre  <Thread> <Precision> ");
        exit(0);
        }
    // on initilise les variables de cacul   
    int precision=atoi(argv[2]),nbr_thread=atoi(argv[1]);    
    std::thread tab[nbr_thread];    
    std::mutex lock;
     
    mpf_init2(somme,precision*4);
    for (int i = 0; i < nbr_thread; i++)
    
       tab[i] = std::thread(pi, precision,(i*precision)/nbr_thread,((i+1)*precision)/nbr_thread,std::ref(lock));
    
    for (int i = 0; i < nbr_thread; i++)
    
           tab[i].join();

    
    /* on affiche le resulat sur la sortie standard , on peut la mettre autre part en modifiant stdout par un fichier     
        avec comme base la base 10 avec comme precision le nombre passé en argument 
    
    */
    mpf_out_str(stdout,10,precision,somme);

    return 0;
}


