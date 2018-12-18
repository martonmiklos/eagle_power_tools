# EPT - EAGLE Power Tools
A Qt app for implementing things for Autodesk EAGLE designfiles/libraries which would be PITA to write in ULP.

WIP (work in progress) features:
 * Quality assurance for libraries
   * Check for silkscreen (t/bPlace) and stop mask overlapping and offer automated slicing to (t/bDocu)
 * Cleanup after importing gerber files:
   * Connecting the overlapping wire segments to a single net
   * Autplacing vias where acceptible
 
Planned features:
 * Quality assurance for libraries
   * Check for parts without name prefix (U$*)
   * Check for power/passive pins suffixed with _1, _2 and offer change to @1, @2... suffix
 * Importing Kicad libraries
 * Sync BoMs to Arena PLM
