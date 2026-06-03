#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "str_manip.h"
#include <direct.h>
#include <errno.h>
#include <locale.h>
#include <windows.h>
#include <ctype.h>

/* ================= CONFIGURATION & UTILITAIRES ================= */

void ensure_data_folders() {
    if (_mkdir(DATA_FOLDER) && errno != EEXIST) perror("Cannot create data folder");
    if (_mkdir(SUB_ACT_FOLDER) && errno != EEXIST) perror("Cannot create Sub_act folder");
    if (_mkdir(SUB_HIST_FOLDER) && errno != EEXIST) perror("Cannot create Sub_hist folder");
}

void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void nettoyer_str(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

int lire_entier() {
    char buffer[100];
    int val = 0;
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &val) != 1) val = 0;
    return val;
}

/* --- SÉCURITÉ : Validation ID --- */
// Renvoie 1 si l'ID est sûr, 0 sinon
int est_id_valide_format(const char *id) {
    if (strlen(id) == 0) return 0;
    for (int i = 0; id[i] != '\0'; i++) {
        // Interdit les espaces et caractères spéciaux de fichiers Windows
        if (isspace(id[i]) || id[i] == '/' || id[i] == '\\' || id[i] == ':' || id[i] == '*')
            return 0;
    }
    return 1;
}

/* ================= PAGE D'AIDE ================= */
void afficher_regles() {
    printf("\n============================================\n");
    printf("         REGLES D'UTILISATION / AIDE        \n");
    printf("============================================\n");
    printf("[!] IMPORTANT POUR EVITER LES BUGS :\n\n");

    printf("1. IDENTIFIANTS (ID) :\n");
    printf("   - Doivent etre UNIQUES.\n");
    printf("   - PAS D'ESPACES (ex: 'Livre1' OK, 'Livre 1' NON).\n");
    printf("   - PAS D'ACCENTS dans les ID.\n\n");

    printf("2. RECHERCHES :\n");
    printf("   - La recherche par titre/nom doit etre exacte (Majuscules/Minuscules).\n\n");

    printf("Appuyez sur Entree pour revenir au menu...");
    getchar();
}

/* ================= MENUS ================= */

/* ================= MENU LIVRES ================= */
void menu_livres(booklist *bl, sublist *sl) {
    int ch = 0;

    while (ch != 11) {
        printf("\n=== GESTION DES LIVRES ===\n");
        printf("1.  Ajouter un livre\n");
        printf("2.  Supprimer un livre par ID\n");
        printf("3.  Afficher tous les livres\n");
        printf("4.  Ajouter des copies (Stock)\n");
        printf("5.  Retirer des copies (Stock)\n");
        printf("6.  Reserver un livre\n");
        printf("7.  Retourner un livre\n");
        printf("8.  RESET (Tout effacer)\n");
        printf("--- RECHERCHE ---\n");
        printf("9.  Rechercher par TITRE\n");
        printf("10. Afficher les livres d'un AUTEUR\n");
        printf("11. Retour au menu principal\n");
        printf("\nVotre choix: ");

        ch = lire_entier();

        switch (ch) {

        case 1: { // AJOUT
            char id[20], ti[100], au[100];
            int n;
            booknode *temp;

            printf("ID (Sans espace, sans accent): ");
            fgets(id, sizeof(id), stdin);
            nettoyer_str(id);

            /* --- SECURITE ID --- */
            if (!est_id_valide_format(id)) {
                printf("[ERREUR] Format ID invalide (pas d'espaces ni caracteres speciaux).\n");
                break;
            }
            if (search_book_by_id(bl, id, &temp)) {
                printf("[ERREUR] Cet ID de livre existe deja !\n");
                break;
            }
            /* ------------------- */

            printf("Titre: ");
            fgets(ti, sizeof(ti), stdin);
            nettoyer_str(ti);

            printf("Auteur: ");
            fgets(au, sizeof(au), stdin);
            nettoyer_str(au);

            printf("Nombre de copies: ");
            n = lire_entier();
            if (n < 0) n = 0;

            add_book_tail(bl, create_book(id, ti, au, n));
            save_booklist(bl);
            printf("Livre ajoute avec succes.\n");
            break;
        }

        case 2: { // SUPPRESSION
            char id[20];
            printf("ID du livre: ");
            fgets(id, sizeof(id), stdin);
            nettoyer_str(id);

            booknode *bn;
            int pos = search_book_by_id(bl, id, &bn);

            if (pos) {
                remove_book_position(bl, pos);
                save_booklist(bl);
                printf("Livre supprime.\n");
            } else {
                printf("Livre introuvable.\n");
            }
            break;
        }

        case 3: // AFFICHER TOUT
            show_all_books_available(bl);
            break;

        case 4: { // AJOUT STOCK
            char id[20];
            int c;
            printf("ID du livre: ");
            fgets(id, sizeof(id), stdin);
            nettoyer_str(id);

            booknode *bn;
            if (!search_book_by_id(bl, id, &bn) || !bn) {
                printf("Livre introuvable.\n");
                break;
            }
            printf("Nombre de copies a ajouter: ");
            c = lire_entier();
            if (c > 0) {
                add_c(&bn->info, c);
                save_booklist(bl);
                printf("Copies ajoutees.\n");
            }
            break;
        }

        case 5: { // RETRAIT STOCK
            char id[20];
            int c;
            printf("ID du livre: ");
            fgets(id, sizeof(id), stdin);
            nettoyer_str(id);

            booknode *bn;
            if (!search_book_by_id(bl, id, &bn) || !bn) {
                printf("Livre introuvable.\n");
                break;
            }
            printf("Nombre de copies a retirer: ");
            c = lire_entier();
            if (c <= 0 || c > bn->info.a_cop) {
                printf("Impossible (Stock insuffisant).\n");
                break;
            }
            remove_c(&bn->info, c);
            save_booklist(bl);
            printf("Copies retirees.\n");
            break;
        }

        case 6: { // RESERVATION
            char bid[20], sid[20];
            printf("ID Livre: ");
            fgets(bid, sizeof(bid), stdin);
            nettoyer_str(bid);

            booknode *bn;
            if (!search_book_by_id(bl, bid, &bn) || !bn) {
                printf("Livre introuvable.\n"); break;
            }

            printf("ID Abonne: ");
            fgets(sid, sizeof(sid), stdin);
            nettoyer_str(sid);

            subnode *sn;
            if (!search_sub_by_id(sl, sid, &sn) || !sn) {
                printf("Abonne introuvable.\n"); break;
            }

            reserve_book_sub(&bn->info, sn->info);
            save_booklist(bl);
            printf("Reservation effectuee.\n");
            break;
        }

        case 7: { // RETOUR
            char bid[20], sid[20];
            printf("ID Livre: ");
            fgets(bid, sizeof(bid), stdin);
            nettoyer_str(bid);

            booknode *bn;
            if (!search_book_by_id(bl, bid, &bn) || !bn) {
                printf("Livre introuvable.\n"); break;
            }

            printf("ID Abonne: ");
            fgets(sid, sizeof(sid), stdin);
            nettoyer_str(sid);

            subnode *sn;
            if (!search_sub_by_id(sl, sid, &sn) || !sn) {
                printf("Abonne introuvable.\n"); break;
            }

            return_book_sub(&bn->info, sn->info);
            save_booklist(bl);
            printf("Livre retourne.\n");
            break;
        }

        case 8: // RESET
             empty_list(bl);
             save_booklist(bl);
             printf("Tous les livres ont ete effaces.\n");
             break;

        case 9: { // RECHERCHE TITRE
            char titre[100];
            printf("Titre exact : ");
            fgets(titre, sizeof(titre), stdin);
            nettoyer_str(titre);

            booknode *trouve = NULL;
            if (search_book_by_name(bl, titre, &trouve) && trouve != NULL) {
                printf("\n--- LIVRE TROUVE ---\n");
                printf("ID     : %s\n", trouve->info.id);
                printf("Titre  : %s\n", trouve->info.title);
                printf("Auteur : %s\n", trouve->info.author);
                printf("Copies : %d / %d\n", trouve->info.a_cop, trouve->info.t_cop);
                printf("--------------------\n");
            } else {
                printf("Aucun livre ne porte ce titre exact.\n");
            }
            break;
        }

        case 10: { // RECHERCHE AUTEUR
            char auteur[100];
            printf("Nom de l'auteur : ");
            fgets(auteur, sizeof(auteur), stdin);
            nettoyer_str(auteur);

            printf("\n--- BIBLIOGRAPHIE : %s ---\n", auteur);
            show_author(bl, auteur);
            printf("-------------------------------\n");
            break;
        }

        case 11:
            printf("Retour au menu principal...\n");
            break;

        default:
            printf("Choix invalide.\n");
        }
    }
}

/* ================= MENU ABONNE ================= */
void menu_subs(sublist *sl) {
    int ch = 0;
    char buffer[100];

    while (ch != 7) {
        printf("\n=== GESTION DES ABONNES ===\n");
        printf("1. Ajouter un abonne\n");
        printf("2. Supprimer un abonne par ID\n");
        printf("3. Afficher tous les abonnes\n");
        printf("4. RESET (Tout effacer)\n");
        printf("5. Afficher l'historique d'un abonne\n");
        printf("--- RECHERCHE ---\n");
        printf("6. Rechercher un abonne par son NOM\n");
        printf("7. Retour au menu principal\n");
        printf("\nVotre choix: ");

        ch = lire_entier();

        switch (ch) {
        case 1: { // AJOUT
            char id[20], nom[100];
            int d, m, y;
            subnode *temp;

            printf("ID (Sans espace, sans accent): ");
            fgets(id, sizeof(id), stdin);
            nettoyer_str(id);

            if (!est_id_valide_format(id)) {
                printf("[ERREUR] ID invalide.\n"); break;
            }
            if (search_sub_by_id(sl, id, &temp)) {
                printf("[ERREUR] ID existant !\n"); break;
            }

            printf("Nom: ");
            fgets(nom, sizeof(nom), stdin);
            nettoyer_str(nom);

            printf("Date de naissance (J M A): ");
            fgets(buffer, sizeof(buffer), stdin);
            d = 0; m = 0; y = 0;
            sscanf(buffer, "%d %d %d", &d, &m, &y);

            date da = create_date(y, m, d);
            if (ver_cor_form(da)) {
                add_sub_tail(sl, create_sub(nom, id, da));
                save_sublist(sl);
                printf("\nAbonne ajoute avec succes !\n");
            } else {
                printf("Date invalide.\n");
            }
            break;
        }
        case 2: { // SUPPRESSION
            char sid[20];
            printf("ID de l'abonne a supprimer: ");
            fgets(sid, sizeof(sid), stdin);
            nettoyer_str(sid);

            subnode *sn;
            int pos = search_sub_by_id(sl, sid, &sn);
            if (pos != 0) {
                remove_sub_position(sl, pos);
                save_sublist(sl);
                printf("Abonne supprime.\n");
            } else {
                printf("Abonne introuvable.\n");
            }
            break;
        }
        case 3: // AFFICHER TOUT
            show_all_subscribers(sl);
            break;
        case 4: // RESET
            empty_sublist(sl);
            save_sublist(sl);
            printf("Tous les abonnes ont ete effaces.\n");
            break;
        case 5: { // HISTORIQUE
            char sid[20];
            printf("ID de l'abonne: ");
            fgets(sid, sizeof(sid), stdin);
            nettoyer_str(sid);

            subnode *sn;
            if (!search_sub_by_id(sl, sid, &sn) || !sn) {
                printf("Abonne introuvable.\n"); break;
            }
            show_sub_history(sn->info);
            break;
        }
        case 6: { // RECHERCHE NOM
            char nom[100];
            char date_str[100];

            printf("Nom de l'abonne : ");
            fgets(nom, sizeof(nom), stdin);
            nettoyer_str(nom);

            subnode *trouve = NULL;
            if (search_sub_by_name(sl, nom, &trouve) && trouve != NULL) {
                printf("\n--- ABONNE TROUVE ---\n");
                printf("ID  : %s\n", trouve->info.id);
                printf("Nom : %s\n", trouve->info.name);
                // On utilise la fonction de date.h pour afficher joliement
                repr_date(trouve->info.birth, date_str);
                printf("Né le : %s\n", date_str);
                printf("---------------------\n");
            } else {
                printf("Aucun abonne trouve avec ce nom.\n");
            }
            break;
        }

        case 7:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
        }
    }
}


/* ================= MAIN ================= */

int main(void) {
    // 1. Configuration Forcee pour les Accents Windows
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);
    setlocale(LC_ALL, "French_France.1252");

    // 2. Initialisation
    ensure_data_folders();
    booklist *books = create_empty_booklist();
    sublist  *subs  = create_empty_sublist();

    load_booklist(books);
    load_sublist(subs);

    int ch = 0;

    while (ch != 4) {
        printf("\n=== BIBLIOTHEQUE ===\n");
        printf("1. Gerer Livres\n");
        printf("2. Gerer Abonnes\n");
        printf("3. Regles et Aide\n");
        printf("4. Quitter et Sauvegarder\n");
        printf("Choix: ");

        scanf("%d", &ch);
        vider_buffer();

        switch (ch) {
        case 1: menu_livres(books, subs); break;
        case 2: menu_subs(subs); break;
        case 3: afficher_regles(); break;
        case 4:
            save_booklist(books);
            save_sublist(subs);
            printf("Sauvegarde terminee. Au revoir.\n");
            break;
        default:
            printf("Choix incorrect.\n");
        }
    }

    // Nettoyage final de la memoire (Optionnel ici car l'OS le fait a la fin)
    // Mais c'est une bonne pratique si vous aviez des fonctions free_list

    return 0;
}
