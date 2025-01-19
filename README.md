# CaluculatingHRR
This program aims to calculate Heat Release Rate by producting OH and CH2O concentration. OH and CH2O is obtained by PLIF measurement. These two pictures have different geometry. 
1. Read image of, chemiluminescence and geometry information.
2. Calculating flame position from chemiluminescence image.
3. Define a common coordinate system from the geometry information described in the scale.json
4. Normalize each image.
5. Calculating HRR by producting each images brightness value.