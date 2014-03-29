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
    translate(v = [ SHELL_SIZE / 3, SHELL_SIZE / 3, CASE_DEPTH / 2 ]) {
      cube(size = [ CASE_WIDTH - (2 * SHELL_SIZE) / 3, CASE_HEIGHT - (2 * SHELL_SIZE) / 3, CASE_DEPTH / 2 ]);
      }
    // Hole for the light
    translate(v = [ CASE_WIDTH / 2, SHELL_SIZE + (1.5 * LIGHT_OUTER_RADIUS), -CASE_DEPTH / 2 ]) {
      cylinder(r = LIGHT_OUTER_RADIUS + 0.5, h = CASE_DEPTH, $fs = RESOLUTION);
      }
    // Hole for the LED
    translate(v = [ CASE_WIDTH - LED_X, CASE_HEIGHT - LED_Y, -CASE_DEPTH / 2 ]) {
      cylinder(r = LED_RADIUS, h = CASE_DEPTH, $fs = RESOLUTION);
      }
    // Hole for the PIR
    translate(v = [ CASE_WIDTH / 4, SHELL_SIZE + (3 * LIGHT_OUTER_RADIUS), -CASE_DEPTH / 2 ]) {
      cylinder(r = PIR_RADIUS, h = CASE_DEPTH, $fs = RESOLUTION);
      }
    // Hole for the LDR
    translate(v = [ CASE_WIDTH / 8, CASE_DEPTH / 2, CASE_DEPTH / 4 ]) {
      rotate(a = [ 90, 0, 0 ]) {
        cylinder(r = LDR_RADIUS, h = CASE_DEPTH, $fs = RESOLUTION);
        }
      }
    }
  // Mount for the bolt
  translate(v = [ CASE_WIDTH / 2, SHELL_SIZE + (3 * LIGHT_OUTER_RADIUS), 0 ]) {
    difference() {
      cylinder(r = (NUT_WIDTH / 2) + 2, h = 10 + SHELL_SIZE, $fs = RESOLUTION);
      translate(v = [ 0, 0, SHELL_SIZE ]) {
        cylinder(r = BOLT_RADIUS, h = 10 + SHELL_SIZE, $fs = RESOLUTION);
        }
      translate(v = [ 0, 0, 10 + SHELL_SIZE ]) {
        hexagon(NUT_WIDTH / 2, 2 * NUT_DEPTH);
        }
      }
    }
  }

