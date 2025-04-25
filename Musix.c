#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_PLAYLISTS 10
#define MAX_SONGS_PER_PLAYLIST 5
#define MAX_SONG_TITLE_LENGTH 100
#define MAX_CATEGORIES 5
#define MAX_SUBCATEGORIES 4
#define MAX_GENRES 5
#define MAX_SUBGENRES 4
#define MAX_RECENT_SEARCHES 20

// Structure Definitions
struct recent_searches {
    char songs[MAX_SONG_TITLE_LENGTH];
};

struct liked_songs {
    char songs[MAX_SONGS_PER_PLAYLIST][MAX_SONG_TITLE_LENGTH];
    int front;
    int rear;
};

struct Playlist {
    char name[MAX_SONG_TITLE_LENGTH];
    char songs[MAX_SONGS_PER_PLAYLIST][MAX_SONG_TITLE_LENGTH];
    int num_songs;
};

struct create_account {
    char name[MAX_SONG_TITLE_LENGTH];
    int age;
    char email[50];
    char pass[10];
    char gender[10];
    long long int mob;
    struct recent_searches *r_s[MAX_RECENT_SEARCHES];
    struct Playlist *play[MAX_PLAYLISTS];
    struct liked_songs *liked;
};

struct Genre {
    char genre_name[20];
    char songs[MAX_SUBGENRES][MAX_SONG_TITLE_LENGTH];
};

struct Categories {
    char category_name[20];
    char categories[MAX_SUBCATEGORIES][20];
};

// Global Variables
struct create_account accounts[MAX_ACCOUNTS];
struct Genre *genres[MAX_GENRES];
struct Categories cate[MAX_CATEGORIES];
struct Playlist fixed_play[3];
int num_accounts = 0;
int top = -1; // For recent searches stack
int top_c_play = -1; // For user playlists
int top_c_song = -1; // For songs in a playlist

// Function Prototypes
void initializeGenres();
void initializeCategories();
void initializeFixedPlaylists();
void saveAccountToFile();
void loadAccountFromFile();
void displayAccount(struct create_account *account);
struct create_account createAccount();
int validatePIN(struct create_account *account, char *enteredPIN);
void like();
int isFull_liked(struct create_account *acc);
int isEmpty_liked(struct create_account *acc);
void enqueue_liked(struct create_account *acc, char *data);
void logic_top5I(struct create_account *acc, int ch);
void logic_top5G(struct create_account *acc, int ch);
void logic_s70(struct create_account *acc, int ch);
void logic_pnb(struct create_account *acc, int ch);
int isFull_song();
int isFull_play();
void create_play_push(struct create_account *acc);
void pop_play(struct Playlist *playlist);
int isFull();
int isEmpty();
void push(struct create_account *acc, char *data);
char *pop(struct create_account *acc, int idx);
void pop_categories(int n);
void pop_genres(int n);
int input(char ch);
void liked_logic(struct create_account *acc, int flag);
void logic(struct create_account *acc, int flag);
void printMainMenu();
void createPlaylist(struct Playlist playlists[], int *num_playlists);
void printPlaylists(struct Playlist playlists[], int num_playlists);
void viewSongs(struct Playlist *playlist);
void addSongToPlaylist(struct Playlist *playlist);
void deletePlaylist(struct Playlist playlists[], int *num_playlists);
void deleteSongFromPlaylist(struct Playlist *playlist);
void searchSong(struct Playlist *playlist);
void sortSongs(struct Playlist *playlist);
void savePlaylistsToFile(struct Playlist playlists[], int num_playlists);
void loadPlaylistsFromFile(struct Playlist playlists[], int *num_playlists);
void freeMemory();

// Initialize Genres
void initializeGenres() {
    for (int i = 0; i < MAX_GENRES; i++) {
        genres[i] = (struct Genre *)malloc(sizeof(struct Genre));
        if (!genres[i]) {
            printf("Memory allocation failed for genres.\n");
            exit(1);
        }
    }
    strcpy(genres[0]->genre_name, "Bollywood");
    strcpy(genres[0]->songs[0], "Saware");
    strcpy(genres[0]->songs[1], "Satranga");
    strcpy(genres[0]->songs[2], "Chaleya");
    strcpy(genres[0]->songs[3], "Rabta");

    strcpy(genres[1]->genre_name, "Slowed reverb");
    strcpy(genres[1]->songs[0], "Piya Ghar Ayenge");
    strcpy(genres[1]->songs[1], "8 Asle");
    strcpy(genres[1]->songs[2], "Attraction");
    strcpy(genres[1]->songs[3], "Shayad");

    strcpy(genres[2]->genre_name, "Soothing_vibes");
    strcpy(genres[2]->songs[0], "Perfect");
    strcpy(genres[2]->songs[1], "Stay");
    strcpy(genres[2]->songs[2], "Hawayein");
    strcpy(genres[2]->songs[3], "Safarnama");

    strcpy(genres[3]->genre_name, "Games");
    strcpy(genres[3]->songs[0], "Counting Stars");
    strcpy(genres[3]->songs[1], "Trapped in my Mind");
    strcpy(genres[3]->songs[2], "Rise Up");
    strcpy(genres[3]->songs[3], "Careless");

    strcpy(genres[4]->genre_name, "Gyms");
    strcpy(genres[4]->songs[0], "Alone");
    strcpy(genres[4]->songs[1], "Beast");
    strcpy(genres[4]->songs[2], "Angetenar");
    strcpy(genres[4]->songs[3], "Infinity");
}

// Initialize Categories
void initializeCategories() {
    strcpy(cate[0].category_name, "Podcasts");
    strcpy(cate[0].categories[0], "News & Politics");
    strcpy(cate[0].categories[1], "Comedy");
    strcpy(cate[0].categories[2], "Education");
    strcpy(cate[0].categories[3], "Sports");

    strcpy(cate[1].category_name, "Instrumental Musics");
    strcpy(cate[1].categories[0], "Classical");
    strcpy(cate[1].categories[1], "Jazz");
    strcpy(cate[1].categories[2], "Blues");
    strcpy(cate[1].categories[3], "World");

    strcpy(cate[2].category_name, "Live Events");
    strcpy(cate[2].categories[0], "Concerts");
    strcpy(cate[2].categories[1], "Festivals");
    strcpy(cate[2].categories[2], "Theater");
    strcpy(cate[2].categories[3], "Sports Events");

    strcpy(cate[3].category_name, "Artists");
    strcpy(cate[3].categories[0], "Rock");
    strcpy(cate[3].categories[1], "Pop");
    strcpy(cate[3].categories[2], "Hip-Hop");
    strcpy(cate[3].categories[3], "Country");

    strcpy(cate[4].category_name, "Audio Books");
    strcpy(cate[4].categories[0], "Fiction");
    strcpy(cate[4].categories[1], "Non-Fiction");
    strcpy(cate[4].categories[2], "Sci-Fi");
    strcpy(cate[4].categories[3], "Biography");
}

// Initialize Fixed Playlists
void initializeFixedPlaylists() {
    strcpy(fixed_play[0].name, "Chill N Thrill");
    fixed_play[0].num_songs = 5;
    strcpy(fixed_play[0].songs[0], "Heroine");
    strcpy(fixed_play[0].songs[1], "Choudhary Jaat Ki");
    strcpy(fixed_play[0].songs[2], "Attraction");
    strcpy(fixed_play[0].songs[3], "9:45");
    strcpy(fixed_play[0].songs[4], "Mi Amor");

    strcpy(fixed_play[1].name, "Study_Fed");
    fixed_play[1].num_songs = 5;
    strcpy(fixed_play[1].songs[0], "Rewrite The Stars");
    strcpy(fixed_play[1].songs[1], "Middle of the Night");
    strcpy(fixed_play[1].songs[2], "Dandelions");
    strcpy(fixed_play[1].songs[3], "Night Changes");
    strcpy(fixed_play[1].songs[4], "MockingBird");

    strcpy(fixed_play[2].name, "Night_Fed");
    fixed_play[2].num_songs = 5;
    strcpy(fixed_play[2].songs[0], "Softly");
    strcpy(fixed_play[2].songs[1], "Drippy");
    strcpy(fixed_play[2].songs[2], "Diamonds");
    strcpy(fixed_play[2].songs[3], "Counting Stars");
    strcpy(fixed_play[2].songs[4], "Wishes");
}

// Save Account to File
void saveAccountToFile() {
    FILE *file = fopen("account.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%d\n", num_accounts);
    for (int i = 0; i < num_accounts; i++) {
        fprintf(file, "%s %s %lld\n", accounts[i].email, accounts[i].pass, accounts[i].mob);
    }

    fclose(file);
    printf("Account saved to file successfully.\n");
}

// Load Account from File
void loadAccountFromFile() {
    FILE *file = fopen("account.txt", "r");
    if (!file) {
        printf("No account file found. Starting fresh.\n");
        return;
    }

    fscanf(file, "%d", &num_accounts);
    for (int i = 0; i < num_accounts && i < MAX_ACCOUNTS; i++) {
        fscanf(file, "%s %s %lld", accounts[i].email, accounts[i].pass, &accounts[i].mob);
    }

    fclose(file);
    printf("Accounts loaded from file successfully.\n");
}

// Display Account
void displayAccount(struct create_account *account) {
    printf("Name: %s\n", account->name);
    printf("Mobile no.: %lld\n", account->mob);
    printf("Email: %s\n", account->email);
    printf("Pin: %s\n", account->pass);

    FILE *file = fopen("display.txt", "a");
    if (!file) {
        printf("Error opening display file.\n");
        return;
    }
    fprintf(file, "Name: %s\nMobile Number: %lld\nEmail: %s\nPin: %s\n\n", 
            account->name, account->mob, account->email, account->pass);
    fclose(file);
}

// Create Account
struct create_account createAccount() {
    struct create_account new_account = {0};
    printf("Name: ");
    scanf(" %[^\n]", new_account.name);

    printf("Age: ");
    scanf("%d", &new_account.age);

gender:
    printf("Enter your gender:\n1. Male\n2. Female\n3. Others\nChoice: ");
    int g;
    scanf("%d", &g);
    switch (g) {
        case 1: strcpy(new_account.gender, "Male"); break;
        case 2: strcpy(new_account.gender, "Female"); break;
        case 3: strcpy(new_account.gender, "Others"); break;
        default: printf("Invalid option.\n"); goto gender;
    }

passcode:
    printf("Set your 6-digit password: ");
    scanf("%s", new_account.pass);
    if (strlen(new_account.pass) != 6) {
        printf("Password must be 6 digits.\n");
        goto passcode;
    }

mobile:
    printf("Mobile number (10 digits): ");
    scanf("%lld", &new_account.mob);
    long long int temp = new_account.mob;
    int digits = 0;
    while (temp > 0) {
        digits++;
        temp /= 10;
    }
    if (digits != 10 || new_account.mob < 6000000000) {
        printf("Invalid mobile number.\n");
        goto mobile;
    }

email:
    printf("E-mail: ");
    scanf("%s", new_account.email);
    int atpos = -1, dotpos = -1;
    int len = strlen(new_account.email);
    for (int i = 0; i < len; i++) {
        if (new_account.email[i] == '@') atpos = i;
        else if (new_account.email[i] == '.') dotpos = i;
    }
    if (atpos < 1 || dotpos < atpos + 2 || dotpos + 2 >= len) {
        printf("Invalid email address.\n");
        goto email;
    }

    new_account.liked = (struct liked_songs *)malloc(sizeof(struct liked_songs));
    if (!new_account.liked) {
        printf("Memory allocation failed for liked songs.\n");
        exit(1);
    }
    new_account.liked->front = -1;
    new_account.liked->rear = -1;

    printf("Account created successfully.\n");
    return new_account;
}

// Validate PIN
int validatePIN(struct create_account *account, char *enteredPIN) {
    return strcmp(account->pass, enteredPIN) == 0;
}

// Initialize Liked Songs
void like() {
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        accounts[i].liked = (struct liked_songs *)malloc(sizeof(struct liked_songs));
        if (!accounts[i].liked) {
            printf("Memory allocation failed for liked songs.\n");
            exit(1);
        }
        accounts[i].liked->front = -1;
        accounts[i].liked->rear = -1;
    }
}

// Liked Songs Queue Operations
int isFull_liked(struct create_account *acc) {
    return acc->liked->rear == MAX_SONGS_PER_PLAYLIST - 1;
}

int isEmpty_liked(struct create_account *acc) {
    return acc->liked->front == -1 || acc->liked->front > acc->liked->rear;
}

void enqueue_liked(struct create_account *acc, char *data) {
    if (isFull_liked(acc)) {
        printf("Liked songs list is full.\n");
        return;
    }
    if (isEmpty_liked(acc)) {
        acc->liked->front = 0;
        acc->liked->rear = 0;
    } else {
        acc->liked->rear++;
    }
    strcpy(acc->liked->songs[acc->liked->rear], data);
    printf("Song added to liked playlist: %s\n", data);
}

// Top 5 Indian Songs Logic
int top5I_pt = 0;
void logic_top5I(struct create_account *acc, int ch) {
    char top_5_I[5][20] = {"Pehle bhi mien", "Gulabi Sadi", "Husn", "Satranga", "Heeriye"};
    if (top5I_pt >= 0 && top5I_pt < 5) {
        switch (ch) {
            case 1: printf("Now playing: %s...\n", top_5_I[top5I_pt]); break;
            case 2:
                if (top5I_pt < 4) printf("Now playing: %s...\n", top_5_I[++top5I_pt]);
                else printf("End of Top 5 Indian songs.\n");
                break;
            case 3:
                if (top5I_pt > 0) printf("Now playing: %s...\n", top_5_I[--top5I_pt]);
                else printf("No previous song.\n");
                break;
            case 4:
                enqueue_liked(acc, top_5_I[top5I_pt]);
                break;
            case 0: printf("Exiting Top 5 Indian songs.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } else {
        printf("Invalid song index.\n");
    }
}

// Top 5 Global Songs Logic
int top5G_pt = 0;
void logic_top5G(struct create_account *acc, int ch) {
    char top_5_G[5][20] = {"Beliver", "Faded", "Toxic", "Montero", "Unholy"};
    if (top5G_pt >= 0 && top5G_pt < 5) {
        switch (ch) {
            case 1: printf("Now playing: %s...\n", top_5_G[top5G_pt]); break;
            case 2:
                if (top5G_pt < 4) printf("Now playing: %s...\n", top_5_G[++top5G_pt]);
                else printf("End of Top 5 Global songs.\n");
                break;
            case 3:
                if (top5G_pt > 0) printf("Now playing: %s...\n", top_5_G[--top5G_pt]);
                else printf("No previous song.\n");
                break;
            case 4:
                enqueue_liked(acc, top_5_G[top5G_pt]);
                break;
            case 0: printf("Exiting Top 5 Global songs.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } else {
        printf("Invalid song index.\n");
    }
}

// 70's Mix Logic
int s70_pt = 0;
void logic_s70(struct create_account *acc, int ch) {
    char s70[5][35] = {"Munna Bada Pyara", "Chura liya hai tumne jo dil ko", 
                       "O Mere Dil ke Chain", "Pal pal dil ke paas", "Neele neele ambar par"};
    if (s70_pt >= 0 && s70_pt < 5) {
        switch (ch) {
            case 1: printf("Now playing: %s...\n", s70[s70_pt]); break;
            case 2:
                if (s70_pt < 4) printf("Now playing: %s...\n", s70[++s70_pt]);
                else printf("End of 70's Mix.\n");
                break;
            case 3:
                if (s70_pt > 0) printf("Now playing: %s...\n", s70[--s70_pt]);
                else printf("No previous song.\n");
                break;
            case 4:
                enqueue_liked(acc, s70[s70_pt]);
                break;
            case 0: printf("Exiting 70's Mix.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } else {
        printf("Invalid song index.\n");
    }
}

// Punjabi Hits Logic
int pnb_pt = 0;
void logic_pnb(struct create_account *acc, int ch) {
    char pnb[5][20] = {"Bandana", "Cheques", "Softly", "Mi Amor", "One Love"};
    if (pnb_pt >= 0 && pnb_pt < 5) {
        switch (ch) {
            case 1: printf("Now playing: %s...\n", pnb[pnb_pt]); break;
            case 2:
                if (pnb_pt < 4) printf("Now playing: %s...\n", pnb[++pnb_pt]);
                else printf("End of Punjabi Hits.\n");
                break;
            case 3:
                if (pnb_pt > 0) printf("Now playing: %s...\n", pnb[--pnb_pt]);
                else printf("No previous song.\n");
                break;
            case 4:
                enqueue_liked(acc, pnb[pnb_pt]);
                break;
            case 0: printf("Exiting Punjabi Hits.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } else {
        printf("Invalid song index.\n");
    }
}

// Playlist Song Operations
int isFull_song() {
    return top_c_song == MAX_SONGS_PER_PLAYLIST - 1;
}

int isFull_play() {
    return top_c_play == MAX_PLAYLISTS - 1;
}

void create_play_push(struct create_account *acc) {
    if (isFull_play()) {
        printf("Maximum playlist capacity reached.\n");
        return;
    }

    char playlist_name[MAX_SONG_TITLE_LENGTH];
    printf("Enter playlist name: ");
    scanf(" %[^\n]", playlist_name);

    top_c_play++;
    acc->play[top_c_play] = (struct Playlist *)malloc(sizeof(struct Playlist));
    if (!acc->play[top_c_play]) {
        printf("Memory allocation failed for playlist.\n");
        top_c_play--;
        return;
    }
    strcpy(acc->play[top_c_play]->name, playlist_name);
    acc->play[top_c_play]->num_songs = 0;

    printf("Enter up to %d songs (enter 'done' to finish):\n", MAX_SONGS_PER_PLAYLIST);
    top_c_song = -1;
    while (!isFull_song()) {
        char song[MAX_SONG_TITLE_LENGTH];
        printf("%d. ", top_c_song + 2);
        scanf(" %[^\n]", song);
        if (strcmp(song, "done") == 0) break;
        strcpy(acc->play[top_c_play]->songs[++top_c_song], song);
        acc->play[top_c_play]->num_songs++;
    }

    printf("Playlist '%s' created successfully.\n", playlist_name);
}

void pop_play(struct Playlist *playlist) {
    printf("Playlist Name: %s\n", playlist->name);
    for (int i = 0; i < playlist->num_songs; i++) {
        printf("%d. %s\n", i + 1, playlist->songs[i]);
    }
}

// Recent Searches Operations
int isFull() {
    return top == MAX_RECENT_SEARCHES - 1;
}

int isEmpty() {
    return top == -1;
}

void push(struct create_account *acc, char *data) {
    if (isFull()) {
        printf("Recent searches list is full.\n");
        return;
    }

    acc->r_s[++top] = (struct recent_searches *)malloc(sizeof(struct recent_searches));
    if (!acc->r_s[top]) {
        printf("Memory allocation failed for recent search.\n");
        top--;
        return;
    }
    strcpy(acc->r_s[top]->songs, data);
    printf("Song Playing: %s\n", acc->r_s[top]->songs);
}

char *pop(struct create_account *acc, int idx) {
    if (idx < 0 || idx > top) return "";
    return acc->r_s[idx]->songs;
}

// Display Categories
void pop_categories(int n) {
    int top_categories = MAX_SUBCATEGORIES - 1;
    switch (n) {
        case 1:
            printf("Podcasts:\n");
            while (top_categories >= 0) printf("%s\n", cate[0].categories[top_categories--]);
            break;
        case 2:
            printf("Instrumental Musics:\n");
            while (top_categories >= 0) printf("%s\n", cate[1].categories[top_categories--]);
            break;
        case 3:
            printf("Live Events:\n");
            while (top_categories >= 0) printf("%s\n", cate[2].categories[top_categories--]);
            break;
        case 4:
            printf("Artists:\n");
            while (top_categories >= 0) printf("%s\n", cate[3].categories[top_categories--]);
            break;
        case 5:
            printf("Audio Books:\n");
            while (top_categories >= 0) printf("%s\n", cate[4].categories[top_categories--]);
            break;
        case 0: printf("Exiting categories.\n"); break;
        default: printf("Invalid option.\n");
    }
}

// Display Genres
void pop_genres(int n) {
    int top_genres = MAX_SUBGENRES - 1;
    switch (n) {
        case 1:
            printf("Bollywood Songs:\n");
            while (top_genres >= 0) printf("%s\n", genres[0]->songs[top_genres--]);
            break;
        case 2:
            printf("Slowed reverb Songs:\n");
            while (top_genres >= 0) printf("%s\n", genres[1]->songs[top_genres--]);
            break;
        case 3:
            printf("Soothing_vibes Songs:\n");
            while (top_genres >= 0) printf("%s\n", genres[2]->songs[top_genres--]);
            break;
        case 4:
            printf("Games Songs:\n");
            while (top_genres >= 0) printf("%s\n", genres[3]->songs[top_genres--]);
            break;
        case 5:
            printf("Gyms Songs:\n");
            while (top_genres >= 0) printf("%s\n", genres[4]->songs[top_genres--]);
            break;
        case 0: printf("Exiting Genres.\n"); break;
        default: printf("Invalid option.\n");
    }
}

// Input Mapping
int input(char ch) {
    switch (ch) {
        case 'a': return 1;
        case 's': return 2;
        case 'w': return 3;
        case 'd': return 4;
        case 'x': return 0;
        default: return -1;
    }
}

// Liked Songs Playback Logic
void liked_logic(struct create_account *acc, int flag) {
    if (isEmpty_liked(acc)) {
        printf("Please like a song first.\n");
        return;
    }

    switch (flag) {
        case 1: printf("Now playing: %s...\n", acc->liked->songs[acc->liked->front]); break;
        case 2:
            if (acc->liked->front < acc->liked->rear) 
                printf("Now playing: %s...\n", acc->liked->songs[++acc->liked->front]);
            else printf("No next song.\n");
            break;
        case 3:
            if (acc->liked->front > 0) 
                printf("Now playing: %s...\n", acc->liked->songs[--acc->liked->front]);
            else printf("No previous song.\n");
            break;
        case 0: printf("Exiting liked songs.\n"); break;
        default: printf("Invalid choice.\n");
    }
}

// Search and Playback Logic
int top_prev = -1;
void logic(struct create_account *acc, int flag) {
    char search[MAX_SONG_TITLE_LENGTH];
    switch (flag) {
        case 1:
            push(acc, fixed_play[rand() % 3].songs[rand() % 5]);
            top_prev = top;
            break;
        case 2:
            if (top_prev > 0) {
                push(acc, acc->r_s[top_prev - 1]->songs);
                top_prev--;
            } else {
                push(acc, fixed_play[rand() % 3].songs[rand() % 5]);
            }
            break;
        case 3:
            printf("Search: ");
            scanf(" %[^\n]", search);
            push(acc, search);
            top_prev = top;
            break;
        case 0: printf("Exiting search.\n"); break;
        default: printf("Invalid choice.\n");
    }
}

// Playlist Management Functions
void printMainMenu() {
    printf("\nGlobal Playlist Menu\n");
    printf("1. Create Playlist\n2. View Playlists\n3. View Songs\n4. Add Song\n");
    printf("5. Delete Playlist\n6. Delete Song\n7. Search Song\n8. Sort Songs\n");
    printf("9. Save Playlists\n10. Load Playlists\n11. Exit\n");
}

void createPlaylist(struct Playlist playlists[], int *num_playlists) {
    if (*num_playlists >= MAX_PLAYLISTS) {
        printf("Maximum playlist limit reached.\n");
        return;
    }
    printf("Enter playlist name: ");
    scanf(" %[^\n]", playlists[*num_playlists].name);
    playlists[*num_playlists].num_songs = 0;
    (*num_playlists)++;
    printf("Playlist created successfully.\n");
}

void printPlaylists(struct Playlist playlists[], int num_playlists) {
    if (num_playlists == 0) {
        printf("No playlists available.\n");
        return;
    }
    printf("\nPlaylists:\n");
    for (int i = 0; i < num_playlists; i++) {
        printf("%d. %s\n", i + 1, playlists[i].name);
    }
}

void viewSongs(struct Playlist *playlist) {
    if (playlist->num_songs == 0) {
        printf("No songs in playlist '%s'.\n", playlist->name);
        return;
    }
    printf("\nSongs in %s:\n", playlist->name);
    for (int i = 0; i < playlist->num_songs; i++) {
        printf("%d. %s\n", i + 1, playlist->songs[i]);
    }
}

void addSongToPlaylist(struct Playlist *playlist) {
    if (playlist->num_songs >= MAX_SONGS_PER_PLAYLIST) {
        printf("Playlist is full.\n");
        return;
    }
    char song[MAX_SONG_TITLE_LENGTH];
    printf("Enter song title: ");
    scanf(" %[^\n]", song);
    strcpy(playlist->songs[playlist->num_songs++], song);
    printf("Song added successfully.\n");
}

void deletePlaylist(struct Playlist playlists[], int *num_playlists) {
    int index;
    printf("Enter playlist number to delete: ");
    scanf("%d", &index);
    index--;
    if (index < 0 || index >= *num_playlists) {
        printf("Invalid playlist number.\n");
        return;
    }
    for (int i = index; i < *num_playlists - 1; i++) {
        playlists[i] = playlists[i + 1];
    }
    (*num_playlists)--;
    printf("Playlist deleted successfully.\n");
}

void deleteSongFromPlaylist(struct Playlist *playlist) {
    int index;
    printf("Enter song number to delete: ");
    scanf("%d", &index);
    index--;
    if (index < 0 || index >= playlist->num_songs) {
        printf("Invalid song number.\n");
        return;
    }
    for (int i = index; i < playlist->num_songs - 1; i++) {
        strcpy(playlist->songs[i], playlist->songs[i + 1]);
    }
    playlist->num_songs--;
    printf("Song deleted successfully.\n");
}

void searchSong(struct Playlist *playlist) {
    char query[MAX_SONG_TITLE_LENGTH];
    printf("Enter search query: ");
    scanf(" %[^\n]", query);
    printf("\nSearch Results in %s:\n", playlist->name);
    int found = 0;
    for (int i = 0; i < playlist->num_songs; i++) {
        if (strstr(playlist->songs[i], query)) {
            printf("%d. %s\n", i + 1, playlist->songs[i]);
            found = 1;
        }
    }
    if (!found) printf("No songs found.\n");
}

void sortSongs(struct Playlist *playlist) {
    for (int i = 0; i < playlist->num_songs - 1; i++) {
        for (int j = 0; j < playlist->num_songs - i - 1; j++) {
            if (strcmp(playlist->songs[j], playlist->songs[j + 1]) > 0) {
                char temp[MAX_SONG_TITLE_LENGTH];
                strcpy(temp, playlist->songs[j]);
                strcpy(playlist->songs[j], playlist->songs[j + 1]);
                strcpy(playlist->songs[j + 1], temp);
            }
        }
    }
    printf("Songs sorted successfully.\n");
}

void savePlaylistsToFile(struct Playlist playlists[], int num_playlists) {
    FILE *file = fopen("playlists.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "%d\n", num_playlists);
    for (int i = 0; i < num_playlists; i++) {
        fprintf(file, "%s %d\n", playlists[i].name, playlists[i].num_songs);
        for (int j = 0; j < playlists[i].num_songs; j++) {
            fprintf(file, "%s\n", playlists[i].songs[j]);
        }
    }
    fclose(file);
    printf("Playlists saved successfully.\n");
}

void loadPlaylistsFromFile(struct Playlist playlists[], int *num_playlists) {
    FILE *file = fopen("playlists.txt", "r");
    if (!file) {
        printf("No playlist file found.\n");
        return;
    }
    fscanf(file, "%d", num_playlists);
    for (int i = 0; i < *num_playlists && i < MAX_PLAYLISTS; i++) {
        fscanf(file, " %[^\n] %d", playlists[i].name, &playlists[i].num_songs);
        for (int j = 0; j < playlists[i].num_songs; j++) {
            fscanf(file, " %[^\n]", playlists[i].songs[j]);
        }
    }
    fclose(file);
    printf("Playlists loaded successfully.\n");
}

// Free Allocated Memory
void freeMemory() {
    for (int i = 0; i < num_accounts; i++) {
        for (int j = 0; j <= top; j++) {
            free(accounts[i].r_s[j]);
        }
        for (int j = 0; j <= top_c_play; j++) {
            free(accounts[i].play[j]);
        }
        free(accounts[i].liked);
    }
    for (int i = 0; i < MAX_GENRES; i++) {
        free(genres[i]);
    }
}

// Main Function
int main() {
    srand(time(NULL));
    like();
    initializeGenres();
    initializeCategories();
    initializeFixedPlaylists();
    loadAccountFromFile();

    struct Playlist global_playlists[MAX_PLAYLISTS];
    int num_playlists = 0;
    loadPlaylistsFromFile(global_playlists, &num_playlists);

    while (1) {
        printf("\nWelcome to Spotify\n");
        printf("1. Create account\n2. Login\n3. Display account\n4. Update account\n0. Exit\n");
        printf("Enter choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (num_accounts < MAX_ACCOUNTS) {
                    accounts[num_accounts++] = createAccount();
                    saveAccountToFile();
                } else {
                    printf("Maximum accounts reached.\n");
                }
                break;

            case 2:
                if (num_accounts == 0) {
                    printf("No accounts available.\n");
                    break;
                }
                {
                    char email[50], pin[10];
                    printf("Enter email: ");
                    scanf("%s", email);
                    int found = 0, acc_index;
                    for (int i = 0; i < num_accounts; i++) {
                        if (strcmp(accounts[i].email, email) == 0) {
                            found = 1;
                            acc_index = i;
                            break;
                        }
                    }
                    if (!found) {
                        printf("Account not found.\n");
                        break;
                    }
                    printf("Enter password: ");
                    scanf("%s", pin);
                    if (!validatePIN(&accounts[acc_index], pin)) {
                        printf("Invalid PIN.\n");
                        break;
                    }

                    int nav;
                    while (1) {
                        printf("\nWelcome to My Musix\n");
                        printf("1. Home\n2. Search\n3. Library\n4. Global Library\n0. Exit\n");
                        printf("Enter choice: ");
                        scanf("%d", &nav);

                        switch (nav) {
                            case 1: {
                                int ch, val = 1;
                                while (val) {
                                    printf("\n🅷 🅾️ 🅼 🅴\n");
                                    printf("1. Liked Songs💗\n2. Top 5 Global🌍\n3. Top 5 Indian🇮🇳\n");
                                    printf("4. Punjabi Hits🥛\n5. 70's MIX📻\n6. Artists\n0. Exit\n");
                                    printf("Enter choice: ");
                                    scanf("%d", &ch);
                                    int gameover = 1;
                                    char option;
                                    switch (ch) {
                                        case 1:
                                            printf("Liked Songs💗\n");
                                            if (isEmpty_liked(&accounts[acc_index])) {
                                                printf("No liked songs.\n");
                                            } else {
                                                printf("Use: a->Play s->Next w->Prev x->Exit\n");
                                                while (gameover) {
                                                    printf("Enter choice: ");
                                                    scanf(" %c", &option);
                                                    gameover = input(option);
                                                    if (gameover == -1) printf("Invalid input.\n");
                                                    else liked_logic(&accounts[acc_index], gameover);
                                                }
                                            }
                                            break;
                                        case 2:
                                            printf("Top 5 Global🌍\n");
                                            printf("1. Beliver\n2. Faded\n3. Toxic\n4. Montero\n5. Unholy\n");
                                            printf("Use: a->Play s->Next w->Prev d->Like x->Exit\n");
                                            while (gameover) {
                                                printf("Enter choice: ");
                                                scanf(" %c", &option);
                                                gameover = input(option);
                                                if (gameover == -1) printf("Invalid input.\n");
                                                else logic_top5G(&accounts[acc_index], gameover);
                                            }
                                            break;
                                        case 3:
                                            printf("Top 5 Indian🇮🇳\n");
                                            printf("1. Pehle bhi mien\n2. Gulabi Sadi\n3. Husn\n4. Satranga\n5. Heeriye\n");
                                            printf("Use: a->Play s->Next w->Prev d->Like x->Exit\n");
                                            while (gameover) {
                                                printf("Enter choice: ");
                                                scanf(" %c", &option);
                                                gameover = input(option);
                                                if (gameover == -1) printf("Invalid input.\n");
                                                else logic_top5I(&accounts[acc_index], gameover);
                                            }
                                            break;
                                        case 4:
                                            printf("Punjabi Hits🥛\n");
                                            printf("1. Bandana\n2. Cheques\n3. Softly\n4. Mi Amor\n5. One Love\n");
                                            printf("Use: a->Play s->Next w->Prev d->Like x->Exit\n");
                                            while (gameover) {
                                                printf("Enter choice: ");
                                                scanf(" %c", &option);
                                                gameover = input(option);
                                                if (gameover == -1) printf("Invalid input.\n");
                                                else logic_pnb(&accounts[acc_index], gameover);
                                            }
                                            break;
                                        case 5:
                                            printf("70's MIX📻\n");
                                            printf("1. Munna Bada Pyara\n2. Chura liya hai tumne jo dil ko\n");
                                            printf("3. O Mere Dil ke Chain\n4. Pal pal dil ke paas\n5. Neele neele ambar par\n");
                                            printf("Use: a->Play s->Next w->Prev d->Like x->Exit\n");
                                            while (gameover) {
                                                printf("Enter choice: ");
                                                scanf(" %c", &option);
                                                gameover = input(option);
                                                if (gameover == -1) printf("Invalid input.\n");
                                                else logic_s70(&accounts[acc_index], gameover);
                                            }
                                            break;
                                        case 6:
                                            printf("Artists:\n1. Arjit Singh\n2. Taylor Swift\n3. Alan Walker\n");
                                            printf("4. Anuv Jain\n5. Darshan Rawal\n");
                                            break;
                                        case 0:
                                            printf("Exiting Home.\n");
                                            val = 0;
                                            break;
                                        default: printf("Invalid choice.\n");
                                    }
                                }
                                break;
                            }
                            case 2: {
                                int ch, val = 1;
                                while (val) {
                                    printf("\n1. Explore Genres\n2. Search\n3. Recent Searches\n4. Explore Categories\n0. Exit\n");
                                    printf("Enter choice: ");
                                    scanf("%d", &ch);
                                    int gameover = 1, opt;
                                    char option;
                                    switch (ch) {
                                        case 1:
                                            printf("Genres:\n1. Bollywood\n2. Slowed reverb\n3. Soothing_vibes\n");
                                            printf("4. Games\n5. Gyms\n0. Exit\n");
                                            while (gameover) {
                                                printf("Enter preference: ");
                                                scanf("%d", &opt);
                                                gameover = opt;
                                                pop_genres(gameover);
                                            }
                                            break;
                                        case 2:
                                            printf("What do you want to listen to?\n");
                                            printf("Use: a->Next s->Previous w->Search x->Exit\n");
                                            while (gameover) {
                                                printf("Enter choice: ");
                                                scanf(" %c", &option);
                                                gameover = input(option);
                                                if (gameover == -1) printf("Invalid input.\n");
                                                else logic(&accounts[acc_index], gameover);
                                            }
                                            break;
                                        case 3:
                                            printf("Recent Searches:\n");
                                            for (int i = top; i >= 0; i--) {
                                                printf("Song: %s\n", pop(&accounts[acc_index], i));
                                            }
                                            if (top == -1) printf("No recent searches.\n");
                                            break;
                                        case 4:
                                            printf("Categories:\n1. Podcasts\n2. Instrumental Music\n3. Live Events\n");
                                            printf("4. Artists\n5. Audio Books\n0. Exit\n");
                                            while (gameover) {
                                                printf("Enter preference: ");
                                                scanf("%d", &opt);
                                                gameover = opt;
                                                pop_categories(gameover);
                                            }
                                            break;
                                        case 0:
                                            printf("Exiting Search.\n");
                                            val = 0;
                                            break;
                                        default: printf("Invalid choice.\n");
                                    }
                                }
                                break;
                            }
                            case 3: {
                                int opt;
                                while (1) {
                                    printf("\n1. Create Playlist\n2. Play Playlist\n0. Exit\n");
                                    printf("Enter choice: ");
                                    scanf("%d", &opt);
                                    switch (opt) {
                                        case 1:
                                            create_play_push(&accounts[acc_index]);
                                            break;
                                        case 2:
                                            if (top_c_play == -1) {
                                                printf("No playlists created.\n");
                                            } else {
                                                for (int j = 0; j <= top_c_play; j++) {
                                                    printf("%d. %s\n", j + 1, accounts[acc_index].play[j]->name);
                                                }
                                                int choice;
                                                printf("Enter playlist number: ");
                                                scanf("%d", &choice);
                                                if (choice < 1 || choice > top_c_play + 1) {
                                                    printf("Invalid choice.\n");
                                                } else {
                                                    pop_play(accounts[acc_index].play[choice - 1]);
                                                }
                                            }
                                            break;
                                        case 0:
                                            printf("Exiting Library.\n");
                                            goto exit_library;
                                        default: printf("Invalid choice.\n");
                                    }
                                }
                            exit_library:
                                break;
                            }
                            case 4: {
                                int choice;
                                while (1) {
                                    printMainMenu();
                                    printf("Enter choice: ");
                                    scanf("%d", &choice);
                                    switch (choice) {
                                        case 1: createPlaylist(global_playlists, &num_playlists); break;
                                        case 2: printPlaylists(global_playlists, num_playlists); break;
                                        case 3: {
                                            int index;
                                            printf("Enter playlist number: ");
                                            scanf("%d", &index);
                                            if (index < 1 || index > num_playlists) printf("Invalid playlist.\n");
                                            else viewSongs(&global_playlists[index - 1]);
                                            break;
                                        }
                                        case 4: {
                                            int index;
                                            printf("Enter playlist number: ");
                                            scanf("%d", &index);
                                            if (index < 1 || index > num_playlists) printf("Invalid playlist.\n");
                                            else addSongToPlaylist(&global_playlists[index - 1]);
                                            break;
                                        }
                                        case 5: deletePlaylist(global_playlists, &num_playlists); break;
                                        case 6: {
                                            int index;
                                            printf("Enter playlist number: ");
                                            scanf("%d", &index);
                                            if (index < 1 || index > num_playlists) printf("Invalid playlist.\n");
                                            else deleteSongFromPlaylist(&global_playlists[index - 1]);
                                            break;
                                        }
                                        case 7: {
                                            int index;
                                            printf("Enter playlist number: ");
                                            scanf("%d", &index);
                                            if (index < 1 || index > num_playlists) printf("Invalid playlist.\n");
                                            else searchSong(&global_playlists[index - 1]);
                                            break;
                                        }
                                        case 8: {
                                            int index;
                                            printf("Enter playlist number: ");
                                            scanf("%d", &index);
                                            if (index < 1 || index > num_playlists) printf("Invalid playlist.\n");
                                            else sortSongs(&global_playlists[index - 1]);
                                            break;
                                        }
                                        case 9: savePlaylistsToFile(global_playlists, num_playlists); break;
                                        case 10: loadPlaylistsFromFile(global_playlists, &num_playlists); break;
                                        case 11: goto exit_global;
                                        default: printf("Invalid choice.\n");
                                    }
                                }
                            exit_global:
                                break;
                            }
                            case 0:
                                printf("Exiting Musix System.\n");
                                goto exit_nav;
                            default: printf("Invalid choice.\n");
                        }
                    }
                exit_nav:
                    break;
                }
                break;

            case 3:
                if (num_accounts == 0) {
                    printf("No accounts available.\n");
                    break;
                }
                {
                    char email[50], pin[10];
                    printf("Enter email: ");
                    scanf("%s", email);
                    int found = 0, acc_index;
                    for (int i = 0; i < num_accounts; i++) {
                        if (strcmp(accounts[i].email, email) == 0) {
                            found = 1;
                            acc_index = i;
                            break;
                        }
                    }
                    if (!found) {
                        printf("Account not found.\n");
                        break;
                    }
                    printf("Enter password: ");
                    scanf("%s", pin);
                    if (!validatePIN(&accounts[acc_index], pin)) {
                        printf("Invalid PIN.\n");
                        break;
                    }
                    displayAccount(&accounts[acc_index]);
                }
                break;

            case 4: {
                char email[50];
                printf("Enter email: ");
                scanf("%s", email);
                int found = 0, acc_index;
                for (int i = 0; i < num_accounts; i++) {
                    if (strcmp(accounts[i].email, email) == 0) {
                        found = 1;
                        acc_index = i;
                        break;
                    }
                }
                if (!found) {
                    printf("Account not found.\n");
                    break;
                }
                char pin[10];
                printf("Enter current password: ");
                scanf("%s", pin);
                if (!validatePIN(&accounts[acc_index], pin)) {
                    printf("Incorrect password.\n");
                    int ch = 1;
                    while (ch) {
                        printf("\n1. Generate OTP via Email\n2. Generate OTP via Mobile\n0. Exit\n");
                        printf("Enter choice: ");
                        scanf("%d", &ch);
                        int randm = rand() % 9999, otp;
                        switch (ch) {
                            case 1: {
                                char email_check[50];
                                printf("Enter registered email: ");
                                scanf("%s", email_check);
                                if (strcmp(accounts[acc_index].email, email_check) != 0) {
                                    printf("Invalid email.\n");
                                    continue;
                                }
                                printf("Your OTP is: %d\n", randm);
                                printf("Enter OTP: ");
                                scanf("%d", &otp);
                                if (otp != randm) {
                                    printf("Invalid OTP.\n");
                                    continue;
                                }
                                printf("Enter new password: ");
                                scanf("%s", accounts[acc_index].pass);
                                if (strlen(accounts[acc_index].pass) != 6) {
                                    printf("Password must be 6 digits.\n");
                                    continue;
                                }
                                printf("Password updated.\n");
                                ch = 0;
                                break;
                            }
                            case 2: {
                                long long int mob;
                                printf("Enter registered mobile: ");
                                scanf("%lld", &mob);
                                if (accounts[acc_index].mob != mob) {
                                    printf("Invalid mobile number.\n");
                                    continue;
                                }
                                printf("Your OTP is: %d\n", randm);
                                printf("Enter OTP: ");
                                scanf("%d", &otp);
                                if (otp != randm) {
                                    printf("Invalid OTP.\n");
                                    continue;
                                }
                                printf("Enter new password: ");
                                scanf("%s", accounts[acc_index].pass);
                                if (strlen(accounts[acc_index].pass) != 6) {
                                    printf("Password must be 6 digits.\n");
                                    continue;
                                }
                                printf("Password updated.\n");
                                ch = 0;
                                break;
                            }
                            case 0: break;
                            default: printf("Invalid choice.\n");
                        }
                    }
                } else {
                    printf("Enter new password: ");
                    scanf("%s", accounts[acc_index].pass);
                    if (strlen(accounts[acc_index].pass) != 6) {
                        printf("Password must be 6 digits.\n");
                        break;
                    }
                    printf("Password updated.\n");
                }
                int save;
                printf("Save changes? 1. Yes 2. No\n");
                scanf("%d", &save);
                if (save == 1) saveAccountToFile();
                break;
            }
            case 0:
                freeMemory();
                printf("Exiting Spotify.\n");
                return 0;
            default: printf("Invalid choice.\n");
        }
    }
}