/* Anna Miller (annam4)
 * ECE 220 - BD2
 *
 * load_tuples opens and reads the input file and uses fscanf to read the information to create
 * a pointer to a linked list. It then calls set_tuples. set_tuples creates a node and then places
 * the node appropriately. If the value is 0, then the function will find a node with matching coordinates
 * and delete it from the list. Otherwise, the node will be placed in the list in order. The order goes
 * row, then, within the row, by column. add_tuples takes 2 matrices and adds them together. It goes
 * through the nodes in the first matrix and copies it to the resulting matrix using set_tuples. It then
 * goes through the second matrix and calls set_tuples to appropriately add the nodes to the result.
 * mult_tuples also takes 2 matrices. It goes through the first matrix and then through the second matrix.
 * All values in the second matrix at rows that match the first matrix column are multipied by the current
 * value of the first matrix. These values are added together and put in the resulting matrix at
 * row = the row of the first matrix and column = the column of the second matrix. Both add_tuples and
 * mult_tuples use gv_tuples. gc_tuples takes the input matrix, row, and column. It goes through the
 * input matrix until it finds a node with a matching row and column, then returns the value of this
 * node. If no node is found, the function returns 0. save_tuples opens an output file to write to, then
 * uses fprintf to write the nodes of the desired matrix to the file with the proper format. destroy_tuples
 * frees all memory allocated to the matrix. It first goes through the individual nodes starting at the
 * head, sets the head to the next node, and frees the current node. This continues until the head pointer
 * is NULL. It then frees the memory allocated to the pointer ot the list.
 */

#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>




sp_tuples * load_tuples(char* input_file)
{
  // hold the row, col, and value of the current tuple
  int r, c;
  double val;

  // open the file to read from
  FILE* file = fopen(input_file, "r");
  if(file == NULL){
    return NULL;
  }

  // allocate memory for the list of tuples
  sp_tuples* list = (sp_tuples *)malloc(sizeof(sp_tuples));

  // set the dimensions of the matrix
  fscanf(file, "%d %d", &(list->m), &(list->n));
  // set the head of the list to the first tuple
  //list->tuples_head = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
  list->tuples_head = NULL;
  list->nz = 0;

  // create the linked list
  while(fscanf(file, "%d %d %lf", &r, &c, &val) > 0){
    // call set_tuples
    set_tuples(list, r, c, val);
  }

  // close the file and return the matrix
  fclose(file);
  return list;
}

double gv_tuples(sp_tuples * mat_t,int row,int col)
{
  // set the current node to the head
  sp_tuples_node* node = mat_t->tuples_head;
  // traverse the list until the end
  while(node){
    // if the node matches the input row and column return the value
    if(node->row == row && node->col == col){
      return node->value;
    }
    // otherwise move to the next node
    node = node->next;
  }
  // if the node is not found, return 0
  return 0;
}


void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
  // set the current node to the beginning of the list
  sp_tuples_node* node = mat_t->tuples_head;
  sp_tuples_node* temp = NULL;

  // allocate memory for the node
  sp_tuples_node* current = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
  current->row = row;
  current->col = col;
  current->value = value;
  current->next = NULL;

  // if the value is 0, delete the appropriate node
  if(value == 0){
    if(node){
      // if the first node should be deleted
      if(node->row  == row && node->col == col){
        // save a pointer to the node to be deleted and set the head to the next node
        temp = node;
        mat_t->tuples_head = node->next;
        // decrement the number of non-zero values
        (mat_t->nz)--;
        // free the memory
        free(temp);
        free(current);
        return;
      }
      // otherwise traverse the list
      while(node->next != NULL){
        // find the node before the one to be deleted
        if(node->next->row == row && node->next->col == col){
          // save a pointer to the node to be deleted and set the previous node to point to the one after
          temp = node->next;
          node->next = node->next->next;
          // decrement the number of non-zero values
          (mat_t->nz)--;
          // free the memory
          free(temp);
          free(current);
          return;
        }
        // try the next node
        node = node->next;
      }
    }
    // if the coordinates do no exist in the list, free the created node
    free(current);
  }
  else{
    // increment the number of non zero values
    (mat_t->nz)++;
    // set node back to the first one
    node = mat_t->tuples_head;
    // if the list is empty, place the node at the beginning
    if(node == NULL){
      mat_t->tuples_head = current;
    }
    // if the list is not empty
    else{
      // if the head node should be moved
      if(node->row > row || (node->row == row && node->col > col)){
        // put the recently added node at the front
        current->next = node;
        mat_t->tuples_head = current;
        return;
      }
      // if the head node should be replaced
      if(node->row == row && node->col == col){
        // set the new value, free the recently created node, and decrement the number of non-zero values
        node->value = value;
        free(current);
        (mat_t->nz)--;
        return;
      }

      // otherwise continue traversing list
      while(node->row <= row && node->next != NULL){
        // if the row and column match the current node, replace the value
        if(node->row == row && node->col == col){
          // set the new value, free the recently added node, and decrement the number of non-zero values
          node->value = value;
          free(current);
          (mat_t->nz)--;
          return;
        }
        // if the row and column match the next node, replace the value
        else if(node->next->row == row && node->next->col == col){
          // set the new value, free the recently added node, and decrement the number of non-zero values
          node->next->value = value;
          free(current);
          (mat_t->nz)--;
          return;
        }
        // if there is no match, find the node that should precede the recently added node
        else if(node->next->row > row || (node->next->row == row && node->next->col >col)){
          // save a pointer to the node that will go after the new node
          temp = node->next;
          // set the next pointer to the new node, and the new node pointing to the next
          node->next = current;
          current->next = temp;
          return;
        }
        // if not found, try the next node
        node = node->next;
      }
      // otherwise add the recently added node to the end
      node->next = current;
      current->next = NULL;
    }
  }
  return;
}


void save_tuples(char * file_name, sp_tuples * mat_t)
{
  // check if the list is empty
  if(mat_t){
    // open the file to write to
    FILE* file = fopen(file_name, "w");

    // write the matrix deimensions to the file
    fprintf(file, "%d %d\n", mat_t->m, mat_t->n);

    // traverse the list and write the nodes
    sp_tuples_node* node = mat_t->tuples_head;
    while(node){
      fprintf(file, "%d %d %lf\n", node->row, node->col, node->value);
      // move to the next node
      node = node->next;
    }
    // close the file
    fclose(file);
  }
  return;
}


sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
  if(matA->m == matB->m && matA->n == matB->n){
    // allocate memory for the result and initialize members
    sp_tuples* retmat = (sp_tuples *)malloc(sizeof(sp_tuples));
    retmat->m = matA->m;
    retmat->n = matA->n;
    retmat->nz = 0;
    retmat->tuples_head = NULL;

    // start at the first nodes of the input matrix
    sp_tuples_node* nodeA = matA->tuples_head;
    sp_tuples_node* nodeB = matB->tuples_head;

    // row, column, and value for the result
    int row, col;
    double value, valueR;

    // traverse the linked list of matrix A
    while(nodeA){
      // set the coordinates
      row = nodeA->row;
      col = nodeA->col;
      value = gv_tuples(matA, row, col);
      set_tuples(retmat, row, col, value);

      // move on to the next node
      nodeA = nodeA->next;
    }

    // traverse linked list of matrix B
    while(nodeB){
      // set the coordinates
      row = nodeB->row;
      col = nodeB->col;
      value = gv_tuples(matB, row, col);
      valueR = gv_tuples(retmat, row, col);

      // set the node to the apporpriate value
      set_tuples(retmat, nodeB->row, nodeB->col, value + valueR);

      // move on to the next node
      nodeB = nodeB->next;
    }
    // if not empty, return the matrix
    return retmat;
  }
  // return NULL if addition not possible
  return NULL;
}



sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){
  // check if multiplication is possible
  if(matA->n == matB->m){
    // allocate memory for the result and initialize members
    sp_tuples* retmat = (sp_tuples *)malloc(sizeof(sp_tuples));
    retmat->m = matA->m;
    retmat->n = matB->n;
    retmat->nz = 0;
    retmat->tuples_head = NULL;

    // start at the first nodes of the input matrix
    sp_tuples_node* nodeA = matA->tuples_head;
    sp_tuples_node* nodeB = matB->tuples_head;

    // traverse the linked list for matrix A
    while(nodeA){
      // multiply the value by each node of B where row = nodeA->column
      nodeB = matB->tuples_head;
        while(nodeB){
          if(nodeB->row == nodeA->col){
            // create a node
            set_tuples(retmat, nodeA->row, nodeB->col, gv_tuples(retmat, nodeA->row, nodeB->col) + nodeA->value * nodeB->value);
          }
          nodeB = nodeB->next;
        }
      // move on to the next node
      nodeA = nodeA->next;
    }

    if(retmat){
      return retmat;
    }
  }
  // return NULL if multiplication is not possible
  return NULL;
}



void destroy_tuples(sp_tuples * mat_t){
  // check if the matrix exists
  if(mat_t){
    // pointer to keep track of the head
    sp_tuples_node* head = mat_t->tuples_head;

    // free memory starting from the first node
    while(head){
      mat_t->tuples_head = head->next;
      free(head);
      head = mat_t->tuples_head;
    }
    // free the memory to the matrix
    free(mat_t);
  }
  return;
}
