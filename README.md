# EPT - EAGLE Power Tools

_The code is heavily WIP considered broken, nonfunctional any time_

A Qt app for implementing things for Autodesk EAGLE designfiles/libraries which would be PITA to write in ULP.

Working features:
 * Converting Kicad libraries
 * Generating stencil (t/bCream) SVG files from boards to be sent to K40 lasercutter
 * Quality assurance for libraries
 
WIP (work in progress) features:
 * Quality assurance for libraries
   * Check for silkscreen (t/bPlace) and stop mask overlapping and offer automated slicing to (t/bDocu)   
 * Cleanup after importing gerber files:
   * Connecting the overlapping wire segments to a single net
   * Autplacing vias where acceptible
 
Planned features:
 * Command line interface for all features
 * Quality assurance for libraries
   * Check for parts without name prefix (U$*)
   * Check for power/passive pins suffixed with _1, _2 and offer change to @1, @2... suffix
   * Check whether all symbols and footprints have >NAME and >VALUE texts on the respective layers
   * Check for '*' texts used for pin 1 marking and replace them with a circle
   * Add feature to perform library QA on design projects: the same checks shall be perfomed on all parts used by a design, with ability to do fixes in the design and the referenced libraries as well
 * Sync BoMs to Arena PLM
 * Generate markdown pages from libraries with symbol/footprint images
 * Connector series generator: draw a connector with least 3 pins, select the pin 1, the middle and the end section and the tool will generate the more pin variants.
