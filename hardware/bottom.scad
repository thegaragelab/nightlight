//---------------------------------------------------------------------------
// Generate the bottom half of the Night Light case
//---------------------------------------------------------------------------
// 27-Mar-2014 ShaneG
//---------------------------------------------------------------------------
include <common.scad>;

union() {
  difference() {
    // Generate the main case
    roundedBox(CASE_WIDTH, CASE_HEIGHT, (CASE_DEPTH / 2) + CASE_OVERLAP, SHELL_RADIUS);
    // Cut out the main shell
    translate(v = [ SHELL_SIZE, SHELL_SIZE, SHELL_SIZE ]) {
      cube(size = [ CASE_WIDTH - (2 * SHELL_SIZE), CASE_HEIGHT - (2 * SHELL_SIZE), CASE_DEPTH ]);
      }
    // Allow for connections
    translate(v = [ 0, 0, CASE_DEPTH / 2 ]) {
      difference() {
        translate(v = [ -SHELL_SIZE / 2, -SHELL_SIZE / 2, 0 ]) {
          cube(size = [ CASE_WIDTH + SHELL_SIZE, CASE_HEIGHT + SHELL_SIZE, CASE_DEPTH / 2 ]);
          }
        translate(v = [ SHELL_SIZE / 2, SHELL_SIZE / 2, -CASE_DEPTH / 2 ]) {
          cube(size = [ CASE_WIDTH - SHELL_SIZE, CASE_HEIGHT - SHELL_SIZE, 2 * CASE_DEPTH ]);
          }
        }
      }
    // Hole for the bolt
    translate(v = [ CASE_WIDTH / 2, SHELL_SIZE + (3 * LIGHT_OUTER_RADIUS), -CASE_DEPTH / 2 ]) {
      cylinder(r = BOLT_RADIUS, h = CASE_DEPTH, $fs = RESOLUTION);
      }
    }
  // Separator panel to hold the PCB in place
  translate(v = [ SHELL_SIZE / 2, CASE_HEIGHT - (2 * SHELL_SIZE) - PCB_HEIGHT, SHELL_SIZE ]) {
    difference() {
      cube(size = [ CASE_WIDTH - SHELL_SIZE, SHELL_SIZE, CASE_DEPTH / 2 ]);
      translate(v = [ (CASE_WIDTH - SHELL_SIZE) / 3, -SHELL_SIZE / 2, -CASE_DEPTH / 4 ]) {
        cube(size = [ (CASE_WIDTH  - SHELL_SIZE) / 3, SHELL_SIZE * 2, CASE_DEPTH ]);
        }
      }
    }
  // Holder for the light
  translate(v = [ CASE_WIDTH / 2, SHELL_SIZE + (1.5 * LIGHT_OUTER_RADIUS), 0 ]) {
    difference() {
      union() {
        difference() {
          cylinder(r = LIGHT_INNER_RADIUS, h = CASE_DEPTH - (2 * SHELL_SIZE), $fs = RESOLUTION);
          translate(v = [ 0, 0, SHELL_SIZE ]) {
            cylinder(r = LIGHT_INNER_RADIUS - SHELL_SIZE, h = CASE_DEPTH - (2 * SHELL_SIZE), $fs = RESOLUTION);
            }
          }
        difference() {
          cylinder(r = LIGHT_OUTER_RADIUS, h = SHELL_SIZE + (PCB_DEPTH - LIGHT_DEPTH), $fs = RESOLUTION);
          translate(v = [ 0, 0, SHELL_SIZE ]) {
            cylinder(r = LIGHT_INNER_RADIUS - SHELL_SIZE, h = SHELL_SIZE + (PCB_DEPTH - LIGHT_DEPTH), $fs = RESOLUTION);
            }
          }
        }
    cube(size = [ SHELL_SIZE, 3 * LIGHT_OUTER_RADIUS, 3 * CASE_DEPTH ], center = true);
      }
    }
  }

