#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI acos ( -1 )
#define DIM 3
#define EPSILON 1e-8
#define RES ( ( unsigned int ) ( -1 ) )


unsigned int    vector_nul ( double vector [ DIM ] );
unsigned int    son_vectors_perpendiculars ( double vector_1 [ DIM ] , double vector_2 [ DIM ] );
unsigned int    vectors_nuls_en_lf_llista ( double llista [ ] [ DIM ] , unsigned int numero_elements );
unsigned int    tipus2d ( 
                    double lf_llista_generadors [ ] [ DIM ] , unsigned int numero_generadors , double vector_perpendicular [ DIM ] , 
                    unsigned int u_llista_generadors_min [ ] , unsigned int *posicio_llista_generadors_min );
unsigned int    projecta_con ( 
                    unsigned int numero_vectors , double vector_ortogonal [ DIM ] , double llista_vectors [ ] [ DIM ] , 
                    double llista_projeccions [ ] [ DIM ] );
unsigned int    tipus3d ( 
                    double lf_llista_generadors [ ] [ DIM ] , unsigned int numero_generadors , unsigned int u_llista_generadors_min [ ] , 
                    unsigned int *posicio_llista_generadors_min );
unsigned int    projeccio_ortogonal_v1_sobre_v2 ( double vector_1 [ DIM ] , double vector_2 [ DIM ] , double proj_v1_sobre_v2 [ DIM ] ); 
unsigned int    calcul_de_variable_angle ( 
                    unsigned int numero_generadors , double *angle , double vector_perpendicular [ DIM ], 
                    double lf_llista_generadors [ ] [ DIM ] , double *angle_max , unsigned int *dreta , unsigned int *esquerra );
unsigned int    minimals_del_pla ( 
                    double lf_llista_generadors [ ] [ DIM ] , unsigned int numero_generadors , unsigned int u_llista_generadors_min [ ] , 
                    unsigned int *posicio_llista_generadors_min , double vector_perpendicular [ DIM ] );
double          producte_escalar ( double vector_1 [ DIM ] , double vector_2 [ DIM ] );
double          norma_vector ( double vector [ DIM ] );
double          determinant ( double vector_1 [ DIM ] , double vector_2 [ DIM ] , double vector_3 [ DIM ] );
double          angle_entre_vectors_signe ( double vector_perpendicular [ DIM ] , double vector_1 [ DIM ] , double vector_2 [ DIM ] );
double          angle_entre_vectors_sense_signe ( double vector_1 [ DIM ] , double vector_2 [ DIM ] );
void            multiplicar_vector_per_escalar ( double vector [ DIM ] , double escalar , double resultat [ DIM ] );
void            resta_vectors ( double vector_1 [ DIM ] , double vector_2 [ DIM ] , double resultat [ DIM ] );
void            afegir_element_u_llista_de_escalars ( 
                    unsigned int *posicio_llista_de_escalars , unsigned int llista_de_escalars [ ] , unsigned int element );
void            afegir_element_lf_llista_de_vectors ( 
                    unsigned int *posicio_llista_de_vectors , double llista_de_vectors [ ] [ DIM ] , double element [ DIM ] );
void            imprimeix_vector_u ( unsigned int vector [ ] , unsigned int dimensio_vector);
void            igualar_vectors_lf ( double vector_1 [ ] , double vector_2 [ ] , unsigned int dimensio );
void            buidar_lf_llista_de_vectors ( 
                    double llista_de_vectors [ ] [ DIM ], unsigned int numero_elements_llista , unsigned int *posicio_llista );
void            buidar_u_llista_de_elements ( 
                    unsigned int llista_de_elements [ ], unsigned int numero_elements_llista , unsigned int *posicio_llista );
void            imprimeix_fitxer_sortida ( 
                    unsigned int u_llista_generadors_min [ ] , double lf_llista_generadors [ ] [ DIM ] , 
                    unsigned int *posicio_llista_generadors_min , char argv [ 2 ] );
void            producte_vectorial ( double vector_1 [ DIM ] , double vector_2 [ DIM ] , double resultat [ DIM ] );
unsigned int    resultat_escrit ( 
                    unsigned int numero_generadors , unsigned int tipus , unsigned int posicio_llista_generadors_min , 
                    unsigned int *u_llista_generadors_min , double ( *lf_llista_generadors ) [ DIM ] , char *argv );

int main( int argc , char * argv [ ] )
{
    if ( argc != 3)
    {
        printf ( 
            "Error: (funció main). Per executar el programa, escriu ''./LujanSergi nom_fitxer_entrada.dat nom_fitxer_sortida.dat'', on"
            " ''nom_fitxer_entrada.dat sigui el nom del fitxer que tingui els vectors generadors, i ''nom_fitxer_sortida'' el nom del fitxer on vols que"
            " s'escriguin els vectors generadors minimals. \n" );
        return ( -1 );
    }

    FILE            *fitxer_entrada;
    unsigned int    j;
    unsigned int    numero_generadors;
    unsigned int    *u_llista_generadors_min;
    unsigned int    posicio_llista_generadors_min;
    unsigned int    *u_llista_temporal;
    unsigned int    tipus;
    double          component_1;
    double          component_2;
    double          component_3;
    double          ( *lf_llista_generadors ) [ DIM ];   
    char            nom_fitxer [ strlen ( argv [ 1 ] ) + 1 ]; // +1 pel caràcter nul.
    
    // Inicialització variables, llistes, fitxers, etc:
    j = 0;
    numero_generadors = 0;
    strcpy ( nom_fitxer , argv [ 1 ] ); 
    fitxer_entrada = fopen ( nom_fitxer , "r" );
    if ( fitxer_entrada == NULL ) 
    {
        printf ( "Error: (funció main). El fitxer no es pot obrir. \n" );
        return ( -1 );
    }
    while ( fscanf ( fitxer_entrada , "%lf,\t%lf,\t%lf" , &component_1 , &component_2 , &component_3 ) == 3 )  
        numero_generadors ++;
    rewind ( fitxer_entrada );
    lf_llista_generadors = ( double ( * ) [ DIM ] ) malloc ( numero_generadors * sizeof ( *lf_llista_generadors ) ); 
    if ( lf_llista_generadors == NULL )
    {
        printf ( "Error: (funció main). El malloc de lf_llista_generadors dona error. \n" );
        return ( -1 );
    }
    u_llista_generadors_min = ( unsigned int ( * ) ) malloc ( numero_generadors * sizeof ( unsigned int ) ); 
    if ( u_llista_generadors_min == NULL )
    {
        printf ( "Error: (funció main). El malloc de u_llista_generadors_min dona error. \n" );
        free ( lf_llista_generadors );
        return ( -1 );
    }
    buidar_u_llista_de_elements ( u_llista_generadors_min , numero_generadors , &posicio_llista_generadors_min );
    while ( fscanf ( fitxer_entrada , "%lf,\t%lf,\t%lf" , &component_1 , &component_2 , &component_3 ) == 3 )  
    {
        if ( component_1 == 0 && component_2 == 0 && component_3 == 0 )
        {
            printf ( "Error: (funció main). Mínim hi ha un vector nul a la llista de vectors introduïda, i això no pot passar. \n" );
            free ( lf_llista_generadors );
            free ( u_llista_generadors_min );
            return ( -1 );
        }
        lf_llista_generadors [ j ] [ 0 ] = component_1;
        lf_llista_generadors [ j ] [ 1 ] = component_2;
        lf_llista_generadors [ j ] [ 2 ] = component_3;
        j ++;
    }
    fclose ( fitxer_entrada );

    // Programa:
    tipus = tipus3d ( lf_llista_generadors , numero_generadors , u_llista_generadors_min , &posicio_llista_generadors_min );
    if ( tipus == ( unsigned int ) ( -1 ) )
    {
        free ( lf_llista_generadors );
        free ( u_llista_generadors_min );
        return ( -1 );
    }
    if ( resultat_escrit ( numero_generadors , tipus , posicio_llista_generadors_min , u_llista_generadors_min , lf_llista_generadors , argv [ 2 ] ) == 1 )
        return ( 0 );
    u_llista_temporal = realloc ( u_llista_generadors_min , posicio_llista_generadors_min * sizeof ( *u_llista_generadors_min ) );
    if ( u_llista_temporal == NULL ) 
    {
        printf ( "Error: (funció main). El realloc de u_llista_temporal dona error. \n" );
        free ( lf_llista_generadors );
        return ( -1 );
    }
    u_llista_generadors_min = u_llista_temporal;
    imprimeix_vector_u ( u_llista_generadors_min , posicio_llista_generadors_min );
    imprimeix_fitxer_sortida ( u_llista_generadors_min , lf_llista_generadors ,  &posicio_llista_generadors_min , argv [ 2 ] );
    free ( lf_llista_generadors );
    free ( u_llista_generadors_min );
    return ( 0 );
}

unsigned int vector_nul ( double vector [ DIM ] )
{
    unsigned int i;
    
    for ( i = 0 ; i < DIM ; i ++ ) 
        if ( fabs ( vector [ i ]) > EPSILON )
            return ( 0 );
    return ( 1 );
}

double producte_escalar ( double vector_1 [ DIM ] , double vector_2 [ DIM ] )
{
    unsigned int    i;
    double          resultat;

    resultat = 0;

    for ( i = 0 ; i < DIM ; i ++ )
        resultat += vector_1 [ i ] * vector_2 [ i ];
    return ( resultat );
}

unsigned int son_vectors_perpendiculars ( double vector_1 [ DIM ] , double vector_2 [ DIM ] )
{
    if ( fabs ( producte_escalar ( vector_1 , vector_2 ) ) < EPSILON )
        return ( 1 );
    return ( 0 );
}

double norma_vector ( double vector [ DIM ] )
{
    return ( sqrt (producte_escalar ( vector , vector ) ) );
}

double determinant ( double vector_1 [ DIM ] , double vector_2 [ DIM ] , double vector_3 [ DIM ] )
{
    double          matriu [ DIM ] [ DIM ];
    unsigned int    i;

    for ( i = 0 ; i < DIM ; i ++ )
        matriu [ i ] [ 0 ] = vector_1 [ i ];
    for ( i = 0 ; i < DIM ; i ++ )
        matriu [ i ] [ 1 ] = vector_2 [ i ];
    for ( i = 0 ; i < DIM ; i ++ )
        matriu [ i ] [ 2 ] = vector_3 [ i ];
    return ( matriu [ 0 ] [ 0 ] * matriu [ 1 ] [ 1 ] * matriu [ 2 ] [ 2 ] + matriu [ 1 ] [ 0 ] * matriu [ 2 ] [ 1 ] * matriu [ 0 ] [ 2 ] +
             matriu [ 0 ] [ 1 ] * matriu [ 1 ] [ 2 ] * matriu [ 2 ] [ 0 ] - matriu [ 0 ] [ 2 ] * matriu [ 1 ] [ 1 ] * matriu [ 2 ] [ 0 ] - 
             matriu [ 1 ] [ 2 ] * matriu [ 2 ] [ 1 ] * matriu [ 0 ] [ 0 ] - matriu [ 0 ] [ 1 ] * matriu [ 1 ] [ 0 ] * matriu [ 2 ] [ 2 ] );
}

double angle_entre_vectors_signe ( double vector_perpendicular [ DIM ] , double vector_1 [ DIM ] , double vector_2 [ DIM ] )
{
    double cos_angle;
    double angle;
    double det_u_v1_v2;

    if ( vector_nul ( vector_1 ) == 1 || vector_nul ( vector_2 ) == 1 )
    {
        printf ( 
            "Error: (funció angle_entre_vectors_signe). Un dels vectors és nul i per tant no es pot calcular "
            "l'angle entre aquests. \n" );
        return ( -10 );
    }
    else
    {
        if (son_vectors_perpendiculars ( vector_perpendicular , vector_1 ) == 1 && 
            son_vectors_perpendiculars ( vector_perpendicular , vector_2 ) == 1 )
        {
            cos_angle = ( producte_escalar ( vector_1 , vector_2 ) ) / ( norma_vector ( vector_1 ) * norma_vector ( vector_2 ) );
            if ( fabs ( cos_angle - 1 ) < EPSILON || cos_angle > 1) 
                cos_angle = 1;
            if ( fabs ( cos_angle + 1 ) < EPSILON || cos_angle < -1  ) 
                cos_angle = -1;
            angle = acos ( cos_angle );
            det_u_v1_v2 = determinant ( vector_perpendicular , vector_1 , vector_2 );
            if ( det_u_v1_v2 < 0 )
                return ( -angle );
            if ( det_u_v1_v2 >= 0 )
                return ( angle ); 
        }
        else 
        {
            printf ( 
                "Error: (funció angle_entre_vectors_signe). L'angle dels vectors que vols calcular no es pot fer,"
                " ja que el vector que ha de ser perpendicular a aquests (per tal de terminar el signe) no ho és. \n" );
            return ( -10 ); 
        }
    }
    printf ( "Error: (funció angle_entre_vectors_signe). La funció ja hauria d'haver retornat anteriorment. \n" );
    return ( -10 );
}

double angle_entre_vectors_sense_signe ( double vector_1 [ DIM ] , double vector_2 [ DIM ] )
{
    double cos_angle;
    double angle;

    if ( vector_nul ( vector_1 ) == 1 || vector_nul ( vector_2 ) == 1 )
    {
        printf ( 
            "Error: (funció angle_entre_vectors_sense_signe). Un dels vectors (o els dos vectors) és nul i per "
            "tant no es pot calcular l'angle entre aquests. \n" );
        return ( -10 );
    }
    cos_angle = ( producte_escalar ( vector_1 , vector_2 ) ) / ( norma_vector ( vector_1 ) * norma_vector ( vector_2 ) );
    if ( fabs ( cos_angle - 1 ) < EPSILON || cos_angle > 1) 
        cos_angle = 1;
    if ( fabs ( cos_angle + 1 ) < EPSILON || cos_angle < -1  ) 
        cos_angle = -1;
    angle = acos ( cos_angle );
    return ( angle );
}

void multiplicar_vector_per_escalar ( double vector [ DIM ] , double escalar , double resultat [ DIM ] )
{
    unsigned int i;

    for ( i = 0 ; i < DIM ; i ++) 
        resultat [ i ] = vector [ i ] * escalar;    
}
    
void resta_vectors ( double vector_1 [ DIM ] , double vector_2 [ DIM ] , double resultat [ DIM ] )
{
    unsigned int i;

    for ( i = 0 ; i < DIM ; i ++ ) 
        resultat [ i ] = vector_1 [ i ] - vector_2 [ i ];
}

unsigned int projeccio_ortogonal_v1_sobre_v2 (                                                          // v1 sobre pla ortogonal de v2
                double vector_1 [ DIM ] , double vector_2 [ DIM ] , double proj_v1_sobre_v2 [ DIM ] ) 
{
    double escalar; 
    double resultat [ DIM ];
    
    if ( vector_nul ( vector_2 ) == 0 )
    {
        escalar = ( producte_escalar ( vector_2 , vector_1 ) ) / ( producte_escalar ( vector_2 , vector_2 ) );
        multiplicar_vector_per_escalar ( vector_2 , escalar , resultat );
        resta_vectors ( vector_1 , resultat , proj_v1_sobre_v2 );
        return ( 0 );
    }
    printf ( "Error: (funció projeccio_ortogonal_v1_sobre_v2). El vector v2 ha de ser diferent de 0.\n" );
    return ( -1 );
}

unsigned int vectors_nuls_en_lf_llista ( double llista [ ] [ DIM ] , unsigned int numero_elements )
{
    unsigned int i;

    for ( i = 0 ; i < numero_elements ; i ++ ) 
    {
        if ( vector_nul ( llista [ i ]) == 1 )
            return ( 1 );
    }
    return ( 0 );
}

void afegir_element_u_llista_de_escalars ( 
        unsigned int *posicio_llista_de_escalars , unsigned int llista_de_escalars [ ] , unsigned int element )
{
    llista_de_escalars [ *posicio_llista_de_escalars ] = element;
    ( *posicio_llista_de_escalars ) ++;
}

void afegir_element_lf_llista_de_vectors ( 
        unsigned int *posicio_llista_de_vectors , double llista_de_vectors [ ] [ DIM ] , double element [ DIM ] )
{
    unsigned int i;

    for ( i = 0 ; i < DIM ; i ++ )
        llista_de_vectors [ *posicio_llista_de_vectors ] [ i ] = element [ i ];
    ( *posicio_llista_de_vectors ) ++;
}

unsigned int tipus2d ( 
                double lf_llista_generadors [ ] [ DIM ] , unsigned int numero_generadors , double vector_perpendicular [ DIM ] , 
                unsigned int u_llista_generadors_min [ ] , unsigned int *posicio_llista_generadors_min )
{
    unsigned int    i;
    unsigned int    j;
    unsigned int    dreta; // Dreta i esquerra guardaran els índexos dels vectors generadors amb angle màxim entre ells.
    unsigned int    esquerra; 
    unsigned int    prova_1; 
    unsigned int    prova_1_vector; 
    unsigned int    prova_2;
    unsigned int    prova_2_vector;
    double          angle_max;
    double          *angle;
    
    // Inicialització variables, llistes, fitxers, etc:
    *posicio_llista_generadors_min = 0;
    prova_1 = 0;
    prova_2 = 0;
    angle = ( double ( * ) ) malloc ( numero_generadors * numero_generadors * sizeof ( double ) ); 
    if ( angle == NULL )
    {
        printf ( "Error: (funció tipus2d). El malloc de angle dona error. \n" );
        return ( -1 );
    }

    // Programa:
    if ( numero_generadors == 0 ) // Cas 0: zero
    {
        free ( angle );
        return ( 0 ); 
    }
    if ( vectors_nuls_en_lf_llista ( lf_llista_generadors , numero_generadors ) == 1 ) 
    {
        printf ( "Error: (funció tipus2d). La llista de vectors que ha rebut tipus2d té mínim un vector nul, i no pot tenir-ne.\n" );
        free ( angle );
        return ( -1 );
    }
    if ( numero_generadors == 1 ) // Cas 1: semirecta
    {
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , 0 );
        free ( angle );
        return ( 1 );
    }
    dreta = 0; 
    esquerra = 1;  
    angle_max = angle_entre_vectors_signe ( vector_perpendicular , lf_llista_generadors [ 0 ] , lf_llista_generadors [ 1 ] );
    if ( angle_max == -10 )
    {
        free ( angle );
        return ( -1 );
    }
    if ( calcul_de_variable_angle (                                                     // Càlcul de tots els angles.
            numero_generadors , angle , vector_perpendicular, lf_llista_generadors , 
            &angle_max , &dreta , &esquerra ) 
        == ( unsigned int ) ( -1 ) )
        return ( -1 );
    if ( angle_max == 0 ) // Cas 1: semirecta
    {
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , 0 );
        free ( angle );
        return ( 1 ); 
    }
    if ( angle_max < PI ) // Implica cas 3 (angle pla) o cas 5 (pla): discutim
    {
        for ( i = 0 ; i < numero_generadors ; i ++ )
        {
            if ( angle [ dreta * numero_generadors + i ] < 0 )  // Implica cas 5, ja que si fos angle pla, 
            {                                                   // no pot passar aquesta condició del if.
                afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , dreta );
                afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , esquerra);
                afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , i ); 
                free ( angle );
                return ( 5 );
            }
        } 
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , dreta ); // Cas 3: angle pla.
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , esquerra );
        free ( angle );
        return ( 3 );
    }
    for ( i = 0 ; i < numero_generadors ; i ++ ) // Aquí, angle_max = PI --> recta, semipla o pla.
    {
        for ( j = 0 ; j < numero_generadors ; j ++ ) 
        {
            if ( angle [ i * numero_generadors + j ] != 0 && angle [ i * numero_generadors + j ] != PI )
            {
                if ( prova_1 == 0 && angle [ dreta * numero_generadors + j ] > 0 && angle [ dreta * numero_generadors + j ] < PI ) 
                {
                    prova_1 = 1;   
                    prova_1_vector = j;  
                }          
                if ( prova_2 == 0 && angle [ dreta * numero_generadors + j ] < 0 && angle [ dreta * numero_generadors + j ] > -PI )
                {
                    prova_2 = 1;
                    prova_2_vector = j;
                }
            }
        }
    }
    if ( prova_1 == 1 && prova_2 == 1 ) // Cas 5: pla
    {

        if ( minimals_del_pla ( 
                lf_llista_generadors , numero_generadors , u_llista_generadors_min , posicio_llista_generadors_min , vector_perpendicular) 
            == ( unsigned int ) ( -1 ) )
            return ( -1 );
        free ( angle );
        return ( 5 );
    }
    if ( prova_1 == 1 && prova_2 == 0 ) // Cas 4: semipla
    {
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , dreta );
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , esquerra );
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , prova_1_vector );
        free ( angle );
        return ( 4 );
    }
    if ( prova_1 == 0 && prova_2 == 1 ) // Cas 4: semipla
    {
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , dreta );
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , esquerra );
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , prova_2_vector );
        free ( angle );
        return ( 4 );
    }
    afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , dreta ); // Cas 2: recta
    afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , esquerra );
    free ( angle );
    return ( 2 ); 
}

void imprimeix_vector_u ( unsigned int vector [ ] , unsigned int dimensio_vector )
{
    unsigned int i;

    if ( dimensio_vector == 1 )
        printf ( " [ %u ]\n\n" , vector [ 0 ] );
    if ( dimensio_vector > 1)
    {  
        printf ( "[ %u " , vector [ 0 ] );
        for ( i = 1 ; i < ( dimensio_vector - 1 ) ; i ++ )
            if ( vector [ i ] != RES )
                printf ( ", %u " , vector [ i ] );
        printf ( ", %u ]\n\n" , vector [ dimensio_vector - 1 ] );
    }
}

unsigned int projecta_con ( 
                unsigned int numero_vectors , double vector_ortogonal [ DIM ] , 
                double llista_vectors [ ] [ DIM ] , double llista_projeccions [ ] [ DIM ] )
{
    unsigned int    i;
    unsigned int    posicio_llista_projeccions;
    double          projeccio [ DIM ];

    posicio_llista_projeccions = 0;

    for ( i = 0 ; i < numero_vectors ; i ++ )
    {
        if ( projeccio_ortogonal_v1_sobre_v2 ( llista_vectors [ i ] , vector_ortogonal , projeccio ) == ( unsigned int ) ( -1 ) )
            return ( -1 );
        if ( vector_nul ( projeccio ) == 0 ) 
            afegir_element_lf_llista_de_vectors ( &posicio_llista_projeccions , llista_projeccions , projeccio );
    }
    return ( posicio_llista_projeccions );
}

unsigned int tipus3d ( 
                double lf_llista_generadors [ ] [ DIM ] , unsigned int numero_generadors , unsigned int u_llista_generadors_min [ ] , 
                unsigned int *posicio_llista_generadors_min )
{
    unsigned int    i; 
    unsigned int    j;
    unsigned int    numero_vectors_projectats;
    unsigned int    vector_repetit;
    unsigned int    *u_llista_generadors_cares;
    unsigned int    posicio_llista_generadors_cares;
    unsigned int    tipus;
    unsigned int    u_llista_vectors_projectats_min [ 4 ];
    unsigned int    posicio_llista_vectors_projectats_min;
    double          ( *lf_llista_vectors_projectats ) [ DIM ];
    unsigned int    posicio_llista_vectors_projectats;
    double          vector_perpendicular [ DIM ];
    double          k;

    // Inicialització variables, llistes, fitxers, etc:
    buidar_u_llista_de_elements ( u_llista_generadors_min , numero_generadors , posicio_llista_generadors_min );
    buidar_u_llista_de_elements ( u_llista_vectors_projectats_min , 4 , &posicio_llista_vectors_projectats_min );

    // Programa:
    if ( numero_generadors == 0 ) // Cas 0: zero
        return ( 0 );
    if ( numero_generadors == 1 ) // Cas 1: semirecta
    {
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , 0 );
        return ( 1 );
    }
    lf_llista_vectors_projectats = ( double ( * ) [ DIM ] ) malloc ( numero_generadors * sizeof ( *lf_llista_vectors_projectats ) ); 
    if ( lf_llista_vectors_projectats == NULL )
    {
        printf ( "Error: (funció tipus3d). El malloc de lf_llista_vectors_projectats dona error. \n" );
        return ( -1 );
    }
    buidar_lf_llista_de_vectors ( lf_llista_vectors_projectats , numero_generadors , &posicio_llista_vectors_projectats );
    u_llista_generadors_cares = ( unsigned int ( * ) ) malloc ( ( numero_generadors - 1 ) * sizeof ( *u_llista_generadors_cares ) ); 
    if ( u_llista_generadors_cares == NULL )
    {
        printf ( "Error: (funció tipus3d). El malloc de u_llista_generadors_cares dona error. \n" );
        free ( lf_llista_vectors_projectats );
        return ( -1 );
    }
    buidar_u_llista_de_elements ( u_llista_generadors_cares , numero_generadors - 1 , &posicio_llista_generadors_cares );
    numero_vectors_projectats = projecta_con (                                          // Ara numero_generadors >= 2.
                                    numero_generadors , lf_llista_generadors [ 0 ] ,    // Aquí no treballem amb posicio_llista_vect_proj 
                                    lf_llista_generadors , lf_llista_vectors_projectats ); // perquè la funció projecta_con funciona diferent. 
    if ( numero_vectors_projectats == ( unsigned int ) ( -1 ) )                                                                                     
    {
        free ( u_llista_generadors_cares );
        free ( lf_llista_vectors_projectats );
        return ( -1 );
    }
    if ( numero_vectors_projectats == 0 ) // Dim con = 1, perquè dim (proj) = 0  --> discutim entre recta o semirecta.
    {
        for ( i = 0 ; i < numero_generadors ; i ++ ) 
        {
            if ( producte_escalar ( lf_llista_generadors [ 0 ] , lf_llista_generadors [ i ] ) < 0 ) // Cas 2: recta.
            {
                afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , 0 );
                afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , i );
                free ( u_llista_generadors_cares );
                free ( lf_llista_vectors_projectats );
                return ( 2 );
            }
        }
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , 0 ); // Cas 1: semirecta.
        free ( u_llista_generadors_cares );
        free ( lf_llista_vectors_projectats );
        return ( 1 );
    }
    tipus = tipus2d (                                                                               
                lf_llista_vectors_projectats , numero_vectors_projectats , lf_llista_generadors [ 0 ] , 
                u_llista_vectors_projectats_min , &posicio_llista_vectors_projectats_min );
    if ( tipus == ( unsigned int ) ( -1 ) )
    {
        free ( u_llista_generadors_cares );
        free ( lf_llista_vectors_projectats );
        return ( -1 );
    }
    if ( tipus < 3 ) // Dim con = 2, perquè dim (proj) = 1. 
    {
        producte_vectorial ( lf_llista_generadors [ 0 ] , lf_llista_generadors [ 1 ] , vector_perpendicular );
        free ( u_llista_generadors_cares );
        free ( lf_llista_vectors_projectats );
        return ( tipus2d ( 
                    lf_llista_generadors , numero_generadors , vector_perpendicular , 
                    u_llista_generadors_min , posicio_llista_generadors_min) );
    }    
    for ( i = 0 ; i < numero_generadors ; i ++ )                                                // Dim con = 3. Classifiquem cada vector en vector  
    {                                                                                           // aresta, vector cara o cap dels 2. En acabar aquest 
        numero_vectors_projectats = projecta_con (                                              // for, a llista_generadors_min únicament tindrem 
                                        numero_generadors , lf_llista_generadors [ i ] ,        // arestes de moment (en el cas que hi hagin).
                                        lf_llista_generadors , lf_llista_vectors_projectats );
        if ( numero_vectors_projectats == ( unsigned int ) ( -1 ) )
        {
            free ( u_llista_generadors_cares );
            free ( lf_llista_vectors_projectats );
            return ( -1 );
        }
        buidar_u_llista_de_elements ( u_llista_vectors_projectats_min , 4 , &posicio_llista_vectors_projectats_min ); 
        tipus = tipus2d ( 
                    lf_llista_vectors_projectats , numero_vectors_projectats , lf_llista_generadors [ i ] , 
                    u_llista_vectors_projectats_min , &posicio_llista_vectors_projectats_min );
        if ( tipus == ( unsigned int ) ( -1 ) )
        {
            free ( u_llista_generadors_cares );
            free ( lf_llista_vectors_projectats );
            return ( -1 );
        }
        if ( tipus == 3 ) // És una aresta --> l'afegim a llista generadors min.
        {
            vector_repetit = 0;
            for ( j = 0 ; j < *posicio_llista_generadors_min; j ++ )
            {
                k = angle_entre_vectors_sense_signe ( lf_llista_generadors [ i ] , lf_llista_generadors [ u_llista_generadors_min [ j ] ] );
                if ( k == -10 )
                {
                    free ( u_llista_generadors_cares );
                    free ( lf_llista_vectors_projectats );
                    return ( -1 );
                }
                if ( k == 0 ) 
                    vector_repetit = 1;
            }
            if ( vector_repetit == 0 )
                afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , i );
        }
        if ( tipus == 4 ) // És un vector cara.
            afegir_element_u_llista_de_escalars ( &posicio_llista_generadors_cares , u_llista_generadors_cares , i );
    } 
    if ( *posicio_llista_generadors_min > 2 ) // Cas 6: con polièdric
    {
        free ( u_llista_generadors_cares );
        free ( lf_llista_vectors_projectats );
        return ( 6 );
    }
    if ( *posicio_llista_generadors_min == 2 )      // Cas 7: con dièdric. Trobem ara dos vectors cara que  
    {                                               // generin el dièdric, i els afegim a llista_generadors_min. 
        igualar_vectors_lf ( vector_perpendicular , lf_llista_generadors [ u_llista_generadors_min [ 0 ] ] , DIM );         // Aresta del dièdric. 
        afegir_element_u_llista_de_escalars ( 
            posicio_llista_generadors_min , u_llista_generadors_min , u_llista_generadors_cares [ 0 ] );    // 1r vector cara del dièdric.         
        buidar_lf_llista_de_vectors (                                                                       // Buidem perquè ja no ens serveix el que 
            lf_llista_vectors_projectats , numero_generadors , &posicio_llista_vectors_projectats );        // hi havia abans i la necessitem nova 
        k = projeccio_ortogonal_v1_sobre_v2 (                                                                              
                lf_llista_generadors [ u_llista_generadors_cares [ 0 ] ] , vector_perpendicular , lf_llista_vectors_projectats [ 0 ] );
        if ( k == ( unsigned int ) ( -1 ) )
        {
            free ( u_llista_generadors_cares );
            free ( lf_llista_vectors_projectats );
            return ( -1 );
        }
        for ( i = 1 ; i < posicio_llista_generadors_cares ; i ++ )
        {
            k = projeccio_ortogonal_v1_sobre_v2 ( 
                    lf_llista_generadors [ u_llista_generadors_cares [ i ] ] , vector_perpendicular , lf_llista_vectors_projectats [ 1 ] );
            if ( k == ( unsigned int ) ( -1 ) )
            {
                free ( u_llista_generadors_cares );
                free ( lf_llista_vectors_projectats );
                return ( -1 );
            }
            k = angle_entre_vectors_sense_signe ( lf_llista_vectors_projectats [ 0 ] , lf_llista_vectors_projectats [ 1 ] );
            if ( k == -10 )
            {
                free ( u_llista_generadors_cares );
                free ( lf_llista_vectors_projectats );
                return ( -1 );
            }
            if ( k != 0 ) // 2n vector cara del dièdric.
            {
                afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , u_llista_generadors_cares [ i ] );
                free ( u_llista_generadors_cares );
                free ( lf_llista_vectors_projectats );
                return ( 7 );
            }
        }
    }
    if ( posicio_llista_generadors_cares > 0 )  // Cas 8: semiespai (perquè té vectors cares). Aquí ja no hi ha arestes (ho sabem perquè   
    {                                           // posició_llista_generadors_min < 2).
        buidar_lf_llista_de_vectors (                                                                   // Ara utilitzem aquesta llista per guardar els 
            lf_llista_vectors_projectats , numero_generadors , &posicio_llista_vectors_projectats );    // vectors cara i un vector interior que li 
        j = numero_generadors - 1;                                                                      // donarà l'alçada al semiespai.
        for ( i = 0 ; i < posicio_llista_generadors_cares ; i ++ )
        {   
            afegir_element_lf_llista_de_vectors ( 
                &posicio_llista_vectors_projectats , lf_llista_vectors_projectats , lf_llista_generadors [ u_llista_generadors_cares [ i ] ] );
            k = fabs ( angle_entre_vectors_sense_signe ( lf_llista_vectors_projectats [ 0 ] , lf_llista_vectors_projectats [ i ] ) );
            if ( k == 10 )
            {
                free ( u_llista_generadors_cares );
                free ( lf_llista_vectors_projectats );
                return ( -1 );
            }
            if ( 0 < k && k < PI ) // Calculem un vector_perpendicular que utilitzarem a posteriori.  
                producte_vectorial ( lf_llista_vectors_projectats [ 0 ] , lf_llista_vectors_projectats [ i ] , vector_perpendicular );                                                    
            if ( j == ( numero_generadors - 1 ) && u_llista_generadors_cares [ i ] > i )    // La condició es dona quan hi ha algún índex dels vectors de     
                j = i;                                                                      // lf_llista_generadors que no s'ha afegit anteriorment a  
        }                                                                                   // llista_generadors_cara pel fet de no ser cara. Aquest serà
        buidar_u_llista_de_elements ( u_llista_vectors_projectats_min , 4 ,                 // el vector que li donarà alçada al semiespai. Si no passa
            &posicio_llista_vectors_projectats_min );                                       // mai --> j=n-1 serà el que dona altura.
        tipus = tipus2d (                                                                    
                    lf_llista_vectors_projectats , posicio_llista_vectors_projectats , vector_perpendicular ,    
                    u_llista_vectors_projectats_min , &posicio_llista_vectors_projectats_min ); // Agafem els minimals dels vect cares que generen el pla.
        if ( tipus == ( unsigned int ) ( -1 ) )
        {
            free ( u_llista_generadors_cares );
            free ( lf_llista_vectors_projectats );
            return ( -1 );
        }
        for ( i = 0 ; i < posicio_llista_vectors_projectats_min ; i ++ )
            afegir_element_u_llista_de_escalars ( 
                posicio_llista_generadors_min , u_llista_generadors_min , u_llista_generadors_cares [ u_llista_vectors_projectats_min [ i ] ] );
        afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , j );
        free ( u_llista_generadors_cares );
        free ( lf_llista_vectors_projectats );
        return ( 8 );
    }
    buidar_lf_llista_de_vectors ( lf_llista_vectors_projectats , numero_generadors , &posicio_llista_vectors_projectats ); // Cas 9: espai
    for ( i = 1 ; i < numero_generadors ; i ++ ) // Afegim tots menys llista_generadors [0].
        afegir_element_lf_llista_de_vectors ( &posicio_llista_vectors_projectats , lf_llista_vectors_projectats , lf_llista_generadors [ i ] );
    i = 0;
    while ( tipus3d ( lf_llista_vectors_projectats , ( numero_generadors - 1 ) , u_llista_generadors_min , posicio_llista_generadors_min) < 9 )
    {
        igualar_vectors_lf ( lf_llista_vectors_projectats [ i ] , lf_llista_generadors [ i ] , DIM );   // A llista_vect_proj, posem llista_gen_min [i],
        i = i + 1;                                                                                      // i treiem el que seria llista_gen_min [i+1].
        if ( i == numero_generadors ) // Passa si no s'en pot treure cap.
        {
            afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , ( numero_generadors - 1 ) );
            free ( u_llista_generadors_cares );
            free ( lf_llista_vectors_projectats );
            return ( 9 );
        }
        buidar_u_llista_de_elements ( u_llista_generadors_min , numero_generadors , posicio_llista_generadors_min ); // Per tornar a cridar tipus3d.
    }
    for ( j = 0 ; j < *posicio_llista_generadors_min ; j ++ )   // Reindexem correctament tots aquells vectors posteriors al que hem tret de la llista.  
    {                                                           // Com que l'últim que treiem és el llista_vect_proj [i], el que ocupi la seva posició
        if ( u_llista_generadors_min [ j ] >= i )               // i els següents aniràn amb un índex menys del que toca.  
            u_llista_generadors_min [ j ] = u_llista_generadors_min [ j ] + 1;  
    }
    free ( u_llista_generadors_cares );
    free ( lf_llista_vectors_projectats );  
    return ( 9 );
}

void igualar_vectors_lf ( double vector_1 [ ] , double vector_2 [ ] , unsigned int dimensio )
{
    unsigned int i; 

    for ( i = 0 ; i < dimensio ; i ++ )
        vector_1 [ i ] = vector_2 [ i ];
}


void buidar_lf_llista_de_vectors ( double llista_de_vectors [ ] [ DIM ], unsigned int numero_elements_llista , unsigned int *posicio_llista )
{
    unsigned int i;
    unsigned int j;

    for ( i = 0 ; i < numero_elements_llista ; i ++ )
        for ( j = 0 ; j < DIM ; j ++ )
            llista_de_vectors [ i ] [ j ] = RES;
    *posicio_llista = 0;
}

void buidar_u_llista_de_elements ( unsigned int llista_de_elements [ ], unsigned int numero_elements_llista , unsigned int *posicio_llista )
{
    unsigned int i;

    for ( i = 0 ; i < numero_elements_llista ; i ++ )
        llista_de_elements [ i ] = RES;
    *posicio_llista = 0;
}

void imprimeix_fitxer_sortida ( 
    unsigned int u_llista_generadors_min [ ] , double lf_llista_generadors [ ] [ DIM ] , 
    unsigned int *posicio_llista_generadors_min , char argv [ 2 ] )
{
    FILE            * fitxer_sortida;
    unsigned int    i;
    unsigned int    j;
    char            nom_fitxer [ strlen ( argv ) + 1 ]; // Pel caràcter nul.

    j = 0;

    strcpy ( nom_fitxer , argv );
    fitxer_sortida = fopen ( nom_fitxer , "w" ); 
    if ( fitxer_sortida == NULL ) 
    {
        fprintf ( stderr , "Error: el fitxer de sortida no es pot obrir. \n" ); 
        return;
    }
    if ( *posicio_llista_generadors_min > 0 )
    {
        for ( i = 0 ; i < *posicio_llista_generadors_min ; i ++ )
        {
            j = u_llista_generadors_min [ i ];
            fprintf ( fitxer_sortida , "%.18lf,\t%.18lf,\t%.18lf" , 
                lf_llista_generadors [ j ] [ 0 ] , lf_llista_generadors [ j ] [ 1 ] , lf_llista_generadors [ j ] [ 2 ] );
            fprintf ( fitxer_sortida , "\n" );
        }
    }
    else
        fprintf ( fitxer_sortida , "No hi ha vectors generadors. \n" );
    fclose ( fitxer_sortida );
}
void producte_vectorial ( double vector_1 [ DIM ] , double vector_2 [ DIM ] , double resultat [ DIM ] )
{
    resultat [ 0 ] = vector_1 [ 1 ] * vector_2 [ 2 ] - vector_1 [ 2 ] * vector_2 [ 1 ];
    resultat [ 1 ] = vector_1 [ 2 ] * vector_2 [ 0 ] - vector_1 [ 0 ] * vector_2 [ 2 ];
    resultat [ 2 ] = vector_1 [ 0 ] * vector_2 [ 1 ] - vector_1 [ 1 ] * vector_2 [ 0 ];
} 

unsigned int calcul_de_variable_angle ( 
    unsigned int numero_generadors , double *angle , double vector_perpendicular [ DIM ], 
    double lf_llista_generadors [ ] [ DIM ] , double *angle_max , unsigned int *dreta , unsigned int *esquerra )
{
    unsigned int i;
    unsigned int j;

    for ( i = 0 ; i < numero_generadors ; i ++ ) 
    {
        for ( j = 0 ; j < numero_generadors ; j ++ )
        {
            angle [ i * ( numero_generadors ) + j ] = angle_entre_vectors_signe ( 
                                                            vector_perpendicular , lf_llista_generadors [ i ] , lf_llista_generadors [ j ] );
            if ( angle [ i * ( numero_generadors ) + j ] == -10 )
            {
                free ( angle );
                return ( -1 );
            }
            if ( angle [ i * ( numero_generadors ) + j ] > *angle_max )
            {
                *dreta = i;
                *esquerra = j;
                *angle_max = angle [ i * ( numero_generadors ) + j ];
            }
        }
    }
    return ( 0 );
}

unsigned int minimals_del_pla ( 
                double lf_llista_generadors [ ] [ DIM ] , unsigned int numero_generadors , unsigned int u_llista_generadors_min [ ] , 
                unsigned int *posicio_llista_generadors_min , double vector_perpendicular [ DIM ] )
{
    unsigned int    i;
    unsigned int    j;
    unsigned int    posicio_llista_vectors_temporals;
    double          ( *lf_llista_vectors_temporal) [ DIM ];

    // Inicialització variables, llistes, fitxers, etc:
    lf_llista_vectors_temporal = ( double ( * ) [ DIM ] ) malloc ( numero_generadors * sizeof ( *lf_llista_vectors_temporal ) ); 
    if ( lf_llista_vectors_temporal == NULL )
    {
        printf ( "Error: (funció minimals_del_pla). El malloc de lf_llista_vectors_temporal dona error. \n" );
        return ( -1 );
    }
    buidar_lf_llista_de_vectors ( lf_llista_vectors_temporal , numero_generadors , &posicio_llista_vectors_temporals );
    

    // Programa:
    for ( i = 1 ; i < numero_generadors ; i ++ ) // Afegim tots menys llista_generadors [0].
        afegir_element_lf_llista_de_vectors ( &posicio_llista_vectors_temporals , lf_llista_vectors_temporal , lf_llista_generadors [ i ] );
    i = 0;
    
    while ( tipus2d ( 
        lf_llista_vectors_temporal , ( numero_generadors - 1 ) , vector_perpendicular , u_llista_generadors_min , posicio_llista_generadors_min ) < 5 )
    {
        igualar_vectors_lf ( lf_llista_vectors_temporal [ i ] , lf_llista_generadors [ i ] , DIM ); // A llista_vect_temp, posem llista_gen_min [i],
        i = i + 1;                                                                                  // i treiem el que seria llista_gen_min [i+1].
        if ( i == numero_generadors ) // Passa si no s'en pot treure cap.
        {
            afegir_element_u_llista_de_escalars ( posicio_llista_generadors_min , u_llista_generadors_min , ( numero_generadors - 1 ) );
            free ( lf_llista_vectors_temporal );
            return ( 5 );
        }
        buidar_u_llista_de_elements ( u_llista_generadors_min , 4 , posicio_llista_generadors_min ); // Per tornar a cridar tipus2d.
    }
    for ( j = 0 ; j < *posicio_llista_generadors_min ; j ++ )   // Reindexem correctament tots aquells vectors posteriors al que hem tret de la llista.  
    {                                                           // Com que l'últim que treiem és el llista_vect_temp [i], el que ocupi la seva posició
        if ( u_llista_generadors_min [ j ] >= i )               // i els següents aniràn amb un índex menys del que toca.  
            u_llista_generadors_min [ j ] = u_llista_generadors_min [ j ] + 1;  
    }
    free ( lf_llista_vectors_temporal );  
    return ( 5 );
}

unsigned int resultat_escrit ( 
                unsigned int numero_generadors , unsigned int tipus , unsigned int posicio_llista_generadors_min , 
                unsigned int *u_llista_generadors_min , double ( *lf_llista_generadors ) [ DIM ] , char *argv )
{
    printf ( "\nS'ha llegit un con generat per %u vectors. \n", numero_generadors );
    printf ( "És un con de tipus ");
    if ( tipus == 0 )
        printf ("{0}.\n");
    if ( tipus == 1 )
        printf ( "semirecta.\n" );
    if ( tipus == 2 )
        printf ( "recta.\n" );
    if ( tipus == 3 )
        printf ( "angle pla.\n" );
    if ( tipus == 4 )
        printf ( "semiplà.\n" );
    if ( tipus == 5 )
        printf ( "pla.\n" );
    if ( tipus == 6 )
        printf ( "polièdric amb %u arestes.\n" , posicio_llista_generadors_min );
    if ( tipus == 7 )
        printf ( "angle dièdric amb 2 arestes i 2 cares.\n" );
    if ( tipus == 8 )
        printf ( "semiespai.\n" );
    if ( tipus == 9 )
        printf ( "espai.\n" );
    printf ( "Un sistema minimal de generadors és: " );
    if ( tipus == 0 )
    {
        printf ( "cap. \n\n" );
        imprimeix_fitxer_sortida ( u_llista_generadors_min , lf_llista_generadors ,  &posicio_llista_generadors_min , argv );
        free ( lf_llista_generadors );
        free ( u_llista_generadors_min );
        return ( 1 );
    }
    return ( 0 );
}

/*
cas 1: tots son necessaris. el que passara es q es treu 1, la funció quan es crida en el while es veu que no genera tot lespai, fa un return <9, i per tant es torna a ficar, es comprova si ja era lultim amb 
    el if del while (suposem que no) i es borra la llista de minimals, i es torna a fer el mateix amb la nova llista, on ara el v_0 ja l'hem ficat i treiem el v_1. es va fent això fins que arribem al final, 
    on sentra en el if del while, safegeix lúltim i es retorna 9, sacaba el programa. 

cas2: en la iteracio 1 de 3dtipus, treiem el v_0, es fa la condicio del while, iteracio 2 del tipus3d. si els que queden tots funcionen, estem en cas 1, sacaba retornant return 9 amb la llista de generadors
correcte, i per tant tornant a la iteració 1 del 3d tipus, no entra en el while i segueix. 

cas 3: en la iteracio 1 de 3d tipus, treiem v_0, es fa la condicio del while, iteracio 2 del tipus 3d, no es produeix cap return que sigui < 9 (aixo implica que v_0 era innecesari pq es segueix produint 
tot lespai ), i per tant arribem a aquesta part del codi, es treu un altre vector, 
ens trobem amb el while, es fa la condicio i seria la 3a iteracio recursiva del 3d tipus, i així succesivament (i si no rebem cap return < 9, voldra dir que tots aquests no seran necesaris). 
com que anem treient vectors, en algun moment ja no es generara tot lespai i fara un return < 9 ( amb una llista de vectors minimals que generen el subespai petit), i llavors s'afegeix aquest ultim vector que haviem
tret i es segueix cridant la funcio tipus 3d, i aixi succesivament.  
sera a partir d'aqui d'on es retorna un return 9 amb llista de generadors ven feta, i reiterativament s'aniran fent return 9 de totes les iteracions recursives que hi han hagut,i no s'entrara en cap while. 
aquesta llista de generadors miniamls ja sera la bona, pq tots els altres vectors de les iteracions anteriors del tipus 3d que hem anat treient, no eren necessaris pq en treurels, seguien generant tot l'espai, 
i per això es tornava a fer una nova iteració de la funció. 

cas 4: el primer innecessari, s'executa la condició del while i no es compleix, i per tant el while es talla. Però ens hem de fixar que alhora d'executar-se la funció
del while, s'ha tornat a cridar un altre cop la funció tipus3d, i per tant s'està fent lo mateix un altre cop però sense tenir en compte aquest vector innecessari, 
i així fins que ens topem amb un vector necessari, que entrem en el while i llavors ja la funció recursiva ja va tirant enrrere. Fixem-nos que en cada iteració
la variable llista_generadors_min es va actualitzant guardant els que realment són necessaris, per això funciona. 

Resum: si els primers són innecessaris, el while es salta però en aplicar la condició del while, la funció s'ha cridat, i per tant en la llista_gen_min aquests vectors innecesaris
que haviem tret de la llista ja no estarà, osigui que funciona. 

Resum resum: treu un vector, si es necessari, el posa i treu el següent, i així successivament. A la que es troba un vector innecesari, ja sortirà al while, pero 
primer es farà la condició, i per tant es tornarà a cridar tipus3d sense aquest vector innecesari, i la llista gen min s'anirà actualitzant i reduint, i si els seguents que 
venen també són innecesaris, llavors lo mateix, s'anirà actualitzant la lista gen min, fins que es trobi amb un necessari, que llavors el posara a la llista i treurà el seguent i quan vagi "tornant" enrrere la 
recursivitat, llavors s'aniran agrupant tots. a la que es troba un bon, els va guardant en llista vect proj, la cual sempre en cada iteració del while, son 
enviats a la funció 3d ,i com són necessaris apareixeran automaticament a llista gen min quan aquesta sigui retornada pel 3d. 

quan es troba un dolent, l'ignora i crida a tipus 3d amb els bons que ja tenia i treient aquest dolent, i així succesivament va treient dolents fins que es troba
amb un bon, que llavors l'afegeix a llista vect proj i per tant amb la seguent iteradcio del while safegeix automaticament a llista gen min. Finalment, 
arribara un punt (el final) on lo que rebi tipus 3d seran tots necessaris i no es podrà treure cap, i llavors, s'afegeix l'ultim (pq no es podia treure) 
i a partir d'aquí es fa el return 9, i per tant tots els whiles anteriors s'aniran trencant. fi. 

un altra manera: es treu un vector, es fa condicio while, es treu un altre y aixi, fins q es treuen tots , i a partir d'aquí anar tirannt enrere.a la q es troba un bo, 
es posa al princpi, i es torna a començar. aixi anem acomulant. 
*/

