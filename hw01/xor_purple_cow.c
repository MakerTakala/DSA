#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct Student {
    uint64_t id;
    uint64_t group;
    struct Student *link;
}Student;

typedef struct Bathroom {
    Student *head;
    Student *tail;
    Student **group_head;
    Student **group_tail;
    Student **next_group;
    Student **prev_group;
}Bathroom;


Bathroom *bathroom;
uint64_t bathroom_size = 0, situation = 0, group_size = 0;
bool *close_bathroom;

Student *XOR( Student *a, Student *b );
void travel();
void enter( uint64_t group_id, uint64_t id, uint64_t bathroom_id );
void leave( uint64_t bathroom_id );
void go( uint64_t bathroom_id );
void close( uint64_t bathroom_id );

int main() {

    scanf("%lu %lu %lu", &bathroom_size, &situation, &group_size);

    bathroom = calloc( sizeof(Bathroom), bathroom_size );
    close_bathroom = calloc( sizeof(bool), bathroom_size );
    for( int i = 0; i < bathroom_size; i++ ) {
        (bathroom + i)->group_head = calloc( sizeof(Student*), group_size );
        (bathroom + i)->group_tail = calloc( sizeof(Student*), group_size );
        (bathroom + i)->next_group = calloc( sizeof(Student*), group_size );
        (bathroom + i)->prev_group = calloc( sizeof(Student*), group_size );
    }

    char command[20] = {0};
    for( int i = 0; i < situation; i++ ) {
        scanf( "%s", command );
        uint64_t group_id = 0, id = 0, bathroom_id = 0;
        if( !strcmp( command, "enter" ) ) {
            scanf( "%lu %lu %lu", &group_id, &id, &bathroom_id );
            enter( group_id, id, bathroom_id );
        }
        else if( !strcmp( command, "leave" ) ) {
            scanf( "%lu", &bathroom_id );
            leave( bathroom_id );
        }
        else if( !strcmp( command, "go" ) ) {
            scanf( "%lu", &bathroom_id );
            go( bathroom_id );
        }
        else {
            scanf( "%lu", &bathroom_id );
            close( bathroom_id );
        }
        printf("\nBATHROOM_HEAD:\n");
        for( int i = 0; i < bathroom_size; i++ ) {
            printf("%p", (bathroom + i)->head );
            if( (bathroom + i)->head  != NULL) {
                printf("(%lu)", (bathroom + i)->head->id);
            }
            printf(" ");
        }
        printf("\n");

        printf("\nHEAD:\n");
        for( int i = 0; i < bathroom_size; i++ ) {
            for( int j = 0; j < group_size; j++ ) {
                printf("%p", *((bathroom + i)->group_head + j) );
                if( *((bathroom + i)->group_head + j) != NULL) {
                    printf("(%lu)", (*((bathroom + i)->group_head + j))->id);
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\nTAIL:\n");
        for( int i = 0; i < bathroom_size; i++ ) {
            for( int j = 0; j < group_size; j++ ) {
                printf("%p", *((bathroom + i)->group_tail + j) );
                if( *((bathroom + i)->group_tail + j) != NULL) {
                    printf("(%lu)", (*((bathroom + i)->group_tail + j))->id);
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\nNEXT_GROUP:\n");
        for( int i = 0; i < bathroom_size; i++ ) {
            for( int j = 0; j < group_size; j++ ) {
                printf("%p", *((bathroom + i)->next_group + j));
                if( *((bathroom + i)->next_group + j) != NULL) {
                    printf("(%lu)", (*((bathroom + i)->next_group + j))->id);
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\nPREV_GROUP:\n");
        for( int i = 0; i < bathroom_size; i++ ) {
            for( int j = 0; j < group_size; j++ ) {
                printf("%p", *((bathroom + i)->prev_group + j));
                if( *((bathroom + i)->prev_group + j) != NULL) {
                    printf("(%lu)", (*((bathroom + i)->prev_group + j))->id);
                }
                printf(" ");
            }
            printf("\n");
        }
        printf("\n");
        travel();
        printf("-------------\n");
    }
    printf("-------------\n");
    travel();
    return 0;
}

Student *XOR( Student *a, Student *b ) {
    return (Student*)(((uint64_t)a) ^ ((uint64_t)b));
}

void travel() {
    for( int i = 0; i < bathroom_size; i++ ) {
        Student *prev = NULL;
        Student *Head = (bathroom + i)->head;
        while( Head ) {
            printf( "%lu(%ld)(%p)(%p) ", Head->id, Head->group, Head->link, Head );
            Student *tmp = Head;
            Head = XOR( Head->link, prev );
            prev = tmp;
        }
        printf( "\n" );
    }
    return;
}

void enter( uint64_t group_id, uint64_t id, uint64_t bathroom_id ) {
    Bathroom *cur_bathroom = bathroom + bathroom_id;
    Student **group_head = cur_bathroom->group_head + group_id;
    Student **group_tail = cur_bathroom->group_tail + group_id;
    Student **next_group = cur_bathroom->next_group + group_id;

    Student *new_student = calloc( sizeof(Student), 1 );
    new_student->id = id;
    new_student->group = group_id;


    if( cur_bathroom->tail == NULL ) {
        new_student->link = NULL;
        cur_bathroom->head = new_student;
        cur_bathroom->tail = new_student;
        *group_head = new_student;
        *group_tail = new_student;
    }
    else if( *group_tail == NULL ){
        *(cur_bathroom->next_group + cur_bathroom->tail->group) = new_student;
        *(cur_bathroom->prev_group + group_id) = cur_bathroom->tail;
        new_student->link = XOR( cur_bathroom->tail, NULL );
        cur_bathroom->tail->link = XOR( cur_bathroom->tail->link, new_student );
        cur_bathroom->tail = new_student;
        *group_head = new_student;
        *group_tail = new_student;
    }
    else if( *group_tail == cur_bathroom->tail ) {
        if( cur_bathroom->tail->group != group_id ) {
            *(cur_bathroom->next_group + cur_bathroom->tail->group) = new_student;
        }
        *(cur_bathroom->prev_group + group_id) = cur_bathroom->tail;
        new_student->link = XOR( cur_bathroom->tail, NULL );
        cur_bathroom->tail->link = XOR( cur_bathroom->tail->link, new_student );
        cur_bathroom->tail = new_student;
        *group_tail = new_student;
    }
    else {
        *(cur_bathroom->prev_group + (*next_group)->group) = new_student;
        (*group_tail)->link = XOR( new_student, XOR( (*group_tail)->link, *next_group ) );
        new_student->link = XOR( *group_tail, *next_group );
        (*next_group)->link = XOR( new_student, XOR( (*next_group)->link, *group_tail ) );
        *group_tail = new_student;
    }
    return;
}
void leave( uint64_t bathroom_id ) {
    Bathroom *cur_bathroom = bathroom + bathroom_id;
    Student *last = cur_bathroom->tail;
    Student *prev = XOR( last->link, NULL );
    Student **group_head = cur_bathroom->group_head + last->group;
    Student **group_tail = cur_bathroom->group_tail + last->group;
    Student **next_group = cur_bathroom->next_group + prev->group;
    Student **prev_groop = cur_bathroom->prev_group + last->group;
    
    if( cur_bathroom->head == cur_bathroom->tail ) {
        cur_bathroom->head = NULL;
        cur_bathroom->tail = NULL;
        *next_group = NULL;
        *prev_groop = NULL;
    }
    else {
        cur_bathroom->tail = prev;
        prev->link = XOR( prev->link, last );
    }
        
    if( *group_head == *group_tail ) {
        printf("SEPT1\n");
        *group_head = NULL;
        *group_tail = NULL;
        *next_group = NULL;
        *prev_groop = NULL;
    }
    else {
        printf("SEPT2\n");
        *group_head = prev;
    }
    printf("SEPT3\n");

    return;
}

void go( uint64_t bathroom_id ) {
    Bathroom *cur_bathroom = bathroom + bathroom_id;
    Student *frist = cur_bathroom->head;
    Student *next = XOR( NULL, frist->link );
    Student **group_head = cur_bathroom->group_head + frist->group;
    Student **group_tail = cur_bathroom->group_tail + frist->group;
    Student **next_group = cur_bathroom->next_group + frist->group;
    Student **prev_group = cur_bathroom->prev_group + next->group;
    
    if( cur_bathroom->head == cur_bathroom->tail ) {
        cur_bathroom->head = NULL;
        cur_bathroom->tail = NULL;
        *next_group = NULL;
        *prev_group = NULL;
    }
    else {
        cur_bathroom->head = next;
        next->link = XOR( frist, next->link );
    }

    if( *group_head == *group_tail ) {
        *group_head = NULL;
        *group_tail = NULL;
        *next_group = NULL;
        *prev_group = NULL;
    }
    else {
        *group_head = next;
    }

    return;
}

void close( uint64_t bathroom_id ) {
    *(close_bathroom + bathroom_id) = true;
    uint64_t next_bathroom_id = bathroom_id;
    while( *(close_bathroom + next_bathroom_id) == true ) {
        next_bathroom_id--;
        next_bathroom_id = (next_bathroom_id + bathroom_size) % bathroom_size;
    }
    Bathroom *leaveing_bathroom = bathroom + bathroom_id;
    Bathroom *entering_bathroom = bathroom + next_bathroom_id;

    while( leaveing_bathroom->tail != NULL ) {
        Student **leaveing_group_head = leaveing_bathroom->group_head + leaveing_bathroom->tail->group;
        Student **leaveing_group_tail = leaveing_bathroom->group_tail + leaveing_bathroom->tail->group;
        Student **entering_group_head = entering_bathroom->group_tail + leaveing_bathroom->tail->group;
        printf("entering_group_head:%p\n", *entering_group_head);

        printf("\n____SETP____ %ld\n", leaveing_bathroom->tail->group);
        

        leaveing_bathroom->tail = *(leaveing_bathroom->prev_group + leaveing_bathroom->tail->group);


        travel();

        if( *leaveing_group_head == *leaveing_group_tail ) {
            enter( (*leaveing_group_head)->group, (*leaveing_group_head)->id, next_bathroom_id );
            leaveing_bathroom->tail->link = XOR( leaveing_bathroom->tail->link, *leaveing_group_head );
            free(*leaveing_group_head);
        }
        else {
            
        }
    
        travel();
        
        
    }
    leaveing_bathroom->head = NULL;

}