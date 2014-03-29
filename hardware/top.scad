//---------------------------------------------------------------------------
// Generate the top half of the Night Light case
//---------------------------------------------------------------------------
// 29-Mar-2014 ShaneG
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
    // Hole for the light
    translate(v = [ CASE_WIDTH / 2, SHELL_SIZE + (1.5 * LIGHT_OUTER_RADIUS), -CASE_DEPTH / 2 ]) {
      cylinder(r = LIGHT_OUTER_RADIUS + 0.5, h = CASE_DEPTH, $fs = RESOLUTION);
      }
    // Hole for the PIR
    translate(v = [ CASE_WIDTH / 4, SHELL_SIZE + (3 * LIGHT_OUTER_RADIUS), -CASE_DEPTH / 2 ]) {
      cylinder(r = PIR_RADIUS, h = CASE_DEPTH, $fs = RESOLUTION);
      }
    // Hole for the LDR
    translate(v = [ CASE_WIDTH / 4, SHELL_SIZE + (3 * LIGHT_OUTER_RADIUS), -CASE_DEPTH / 2 ]) {
      cylinder(r = LDR_RADIUS, h = CASE_DEPTH, $fs = RESOLUTION);
      }
    }
  // Mount for the bolt
  translate(v = [ CASE_WIDTH / 2, SHELL_SIZE + (3 * LIGHT_OUTER_RADIUS), 0 ]) {
    difference() {
      cylinder(r = (NUT_WIDTH / 2) + 2, h = 10 + SHELL_SIZE, $fs = RESOLUTION);
      translate(v = [ 0, 0, SHELL_SIZE ]) {
        cylinder(r = BOLT_RADIUS, h = 10 + SHELL_SIZE, $fs = RESOLUTION);
        }
      translate(v = [ 0, 0, 10 + SHELL_SIZE - NUT_DEPTH ]) {
        hexagon(length = NUT_SIZE, depth = 6 * NUT_DEPTH);
        }
      }
    }
  }

/*
  // Mount for the bolt
  translate(v = [ CASE_WIDTH / 2, SHELL_SIZE + (3 * LIGHT_OUTER_RADIUS), 0 ]) {
    difference() {
      cylinder(r = (NUT_WIDTH / 2) + 2, h = 10 + SHELL_SIZE, $fs = RESOLUTION);
      translate(v = [ 0, 0, SHELL_SIZE ]) {
        cylinder(r = BOLT_RADIUS, h = 10 + SHELL_SIZE, $fs = RESOLUTION);
        }
      translate(v = [ 0, 0, 10 + SHELL_SIZE - NUT_DEPTH ]) {
        hexagon(length = NUT_SIZE, depth = 6 * NUT_DEPTH);
        }
      }
    }
*/
