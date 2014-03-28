//---------------------------------------------------------------------------
// Common definitions for the Night Light case
//---------------------------------------------------------------------------
// 27-Mar-2014 ShaneG
//
// This file contains the common definitions (dimensions and utility modules)
// used to generate the two halves of the Night Light case.
//---------------------------------------------------------------------------

// Printer resolution (used for spheres and circles)
RESOLUTION = 0.1;

// Shell information
SHELL_SIZE   = 3;
SHELL_RADIUS = 2;

// Size for the board + battery pack
PCB_WIDTH  = 52;
PCB_HEIGHT = 54;
PCB_DEPTH  = 28;

// Light head dimensions
LIGHT_OUTER_RADIUS = 29 / 2;
LIGHT_INNER_RADIUS = 25 / 2;
LIGHT_DEPTH        = 20;

// PIR sensor
PIR_RADIUS = 10 / 2;
PIR_DEPTH  = 20;

// LDR
LDR_RADIUS = 5 / 2;

// Bolts and nuts
BOLT_RADIUS = 3 / 2;
BOLT_DEPTH  = 25;
NUT_WIDTH   = 5.2;

// Case dimensions
CASE_WIDTH   = PCB_WIDTH + (2 * SHELL_SIZE);
CASE_HEIGHT  = PCB_HEIGHT + (4 * LIGHT_OUTER_RADIUS) + (2 * SHELL_SIZE);
CASE_DEPTH   = PCB_DEPTH + (2 * SHELL_SIZE);
CASE_OVERLAP = 5;

//---------------------------------------------------------------------------
// Modules
//---------------------------------------------------------------------------

// Generate a shape that can be extracted from a cube to generate a rounded
// edge.
module roundedEdge(length, radius) {
  difference() {
    translate(v = [ radius, radius, 0 ]) {
      cube(size = [ radius * 2, radius * 2, length ], center = true);
      }
    cylinder(r = radius, h = length + 1, center = true, $fs = RESOLUTION);
    }
  }

// Generate a shape that can be extracted from a cube to generate a rounded
// corner.
module roundedCorner(radius) {
  difference() {
    cube(size = [ radius * 2, radius * 2, radius * 2 ]);
    sphere(r = radius, $fs = RESOLUTION);
    }
  }

// Generate a box with rounded edges and corners
module roundedBox(width, height, depth, radius) {
  translate(v = [ width, 0, depth ]) {
    rotate(a = [ 0, 180, 0 ]) {
      difference() {
        // Start with the cube
        cube(size = [ width, height, depth ]);
        // Substract the edges
        translate(v = [ width - radius, 0, depth - radius ]) {
          rotate(a = [ 90, 0, 0 ]) {
            roundedEdge(height * 3, radius);
            }
          }
        translate(v = [ radius, 0, depth - radius ]) {
          rotate(a = [ 90, 0, 0 ]) {
            rotate(a = [ 0, 0, 90 ]) {
              roundedEdge(height * 3, radius);
              }
            }
          }
        translate(v = [ 0, radius, depth - radius ]) {
          rotate(a = [ 0, 90, 0 ]) {
            rotate(a = [ 0, 0, 180 ]) {
              roundedEdge(height * 3, radius);
              }
            }
          }
        translate(v = [ 0, height - radius, depth - radius ]) {
          rotate(a = [ 0, -90, 0 ]) {
            roundedEdge(height * 3, radius);
            }
          }
        translate(v = [ radius, radius, 0 ]) {
          rotate(a = [ 0, 0, 180 ]) {
            roundedEdge(height * 3, radius);
            }
          }
        translate(v = [ width - radius, radius, 0 ]) {
          rotate(a = [ 0, 0, -90 ]) {
            roundedEdge(height * 3, radius);
            }
          }
        translate(v = [ width - radius, height - radius, 0 ]) {
          roundedEdge(height * 3, radius);
          }
        translate(v = [ radius, height - radius, 0 ]) {
          rotate(a = [ 0, 0, 90 ]) {
            roundedEdge(height * 3, radius);
            }
          }
        // Substract the corners
        translate(v = [ width - radius, height - radius, depth - radius ]) {
          roundedCorner(radius);
          }
        translate(v = [ radius, height - radius, depth - radius ]) {
          rotate(a = [ 0, 0, 90 ]) {
            roundedCorner(radius);
            }
          }
        translate(v = [ radius, radius, depth - radius ]) {
          rotate(a = [ 0, 0, 180 ]) {
            roundedCorner(radius);
            }
          }
        translate(v = [ width - radius, radius, depth - radius ]) {
          rotate(a = [ 0, 0, 270 ]) {
            roundedCorner(radius);
            }
          }
        }
      }
    }
  }

