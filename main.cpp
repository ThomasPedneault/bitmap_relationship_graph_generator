#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "bitmap_image.hpp"

#define PI 3.14159265

struct Color {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

bool operator==(const Color& lhs, const Color& rhs) {
  return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
}

struct Node {
  Color value;
  int x;
  int y;
};

Node* find_node_in_arr(Node** arr, int size, const Color& value) {
  Node* nptr = nullptr;
  for(int i = 0; i < size; i++) {
    if(arr[i]->value == value) nptr = arr[i];
  }
  return nptr;
}

const int RADIUS = 200;
const int NODE_RADIUS = 12;
const int NUMBER_OF_NODES_TO_GENERATE = 10; // for testing only, change this to the number of nodes to create.
const int NUMBER_OF_RELATIONSHIPS_TO_GENERATE = 25 * 2; // for testing only, change the number on the left to the number of relationship to create. (ex: 5 relationships will be 5 * 2) the multiplication is because one relationship needs 2 nodes.

int main() {
  bitmap_image bmp(512, 512);
  bmp.set_all_channels(255, 150, 50);

  srand(time(NULL));  // init rng with a random seed.

  Color node_values[NUMBER_OF_NODES_TO_GENERATE];
  for(int i = 0; i < NUMBER_OF_NODES_TO_GENERATE; i++) {
    Color color {
      rand() % 256,
      rand() % 256,
      rand() % 256
    };
    node_values[i] = color;
  }

  Color node_relationships[NUMBER_OF_RELATIONSHIPS_TO_GENERATE];
  for(int i = 0; i < NUMBER_OF_RELATIONSHIPS_TO_GENERATE; i+=2) {
    int n1_index = rand() % NUMBER_OF_NODES_TO_GENERATE;
    int n2_index;
    do {
	n2_index = rand() % NUMBER_OF_NODES_TO_GENERATE;
    } while(n1_index == n2_index);
    node_relationships[i] = node_values[n1_index];
    node_relationships[i+1] = node_values[n2_index];
  }

  int node_count = sizeof(node_values)/sizeof(node_values[0]);
  Node** node_arr = new Node*[node_count];
  for(int i = 0; i < node_count; i++) {
    node_arr[i] = new Node { node_values[i], 0, 0 };
  }

  image_drawer draw(bmp);
  draw.pen_width(3);
  draw.pen_color(255, 0, 0);
  draw.circle(bmp.width() / 2, bmp.height() / 2, RADIUS);

  float theta = 0.0;
  float angle_offset = 360.0 / node_count;
  for(int i = 0; i < node_count; i++) {
    // unit circle magic
    Node* node = node_arr[i];
    node->x = bmp.width() / 2 + cos(theta*PI/180) * RADIUS;
    node->y = bmp.height() / 2 + sin(theta*PI/180) * RADIUS;
    theta += angle_offset;

    draw.pen_width(5);
    draw.pen_color(node->value.red, node->value.green, node->value.blue);
    draw.circle(node_arr[i]->x, node_arr[i]->y, NODE_RADIUS);
  }

  draw.pen_color(255, 255, 255);

  int rel_size = sizeof(node_relationships) / sizeof(node_relationships[0]);
  for(int i = 0; i < rel_size; i+=2) {
    Node* n1 = find_node_in_arr(node_arr, node_count, node_relationships[i]);
    Node* n2 = find_node_in_arr(node_arr, node_count, node_relationships[i+1]);
    draw.line_segment(n1->x, n1->y, n2->x, n2->y);
  }

  delete[] node_arr;

  bmp.save_image("graph.bmp");

  return 0;
}
