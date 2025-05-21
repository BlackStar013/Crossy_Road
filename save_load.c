#include "save_load.h"

void save(int xp, int highscore){
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "xp", xp);
    cJSON_AddNumberToObject(json, "highscore", highscore);
    char *json_str = cJSON_Print(json);
    FILE *fptr = fopen("data/save.json", "w");
    fputs(json_str, fptr);
    fclose(fptr);
    cJSON_free(json_str);
    cJSON_Delete(json);
}

void load(int* xp, int* highscore){
    FILE *fptr;
    fptr = fopen("data/save.json", "r");
    if (fptr == NULL) {
        printf("Error: Unable to open the file.\n");
    }
    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), fptr);
    fclose(fptr);
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
    }
    cJSON *xpj = cJSON_GetObjectItemCaseSensitive(json, "xp");
    cJSON *highscorej = cJSON_GetObjectItemCaseSensitive(json, "highscore");
    //printf("%d\n", (xpj != NULL));
    *xp = xpj->valueint;
    *highscore = highscorej->valueint;
    cJSON_Delete(json);
}

/* int main(void){
    save(10, 200);
    int xp;
    int highscore;
    load(&xp, &highscore);
    printf("xp: %d, highscore: %d\n",xp,highscore);
}
 */