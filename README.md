# Electron AI PID

## Generate events
The Generate_Single_Particle.C code is used to generate a flat distribution of Pion/lepton. In each events, a lepton and a pion are generate in opposite sectors. This allows to somulate both kind of partile at the same time and save some processing time. One can also simulate single particle events by modifying the code.
To run it, first create an output folder
```
mkdir Gen_folder_posi
```
Then modify the outFolder variable in the code accordingly
Run the code as:
```
root -l Generate_Single_particle.C
```

##Reconstruct the events
Use OSG to process the produced lund files

##Analyze events and produce training trees
To produce the training trees countaining the training variable run:
```
clas12root -l Process_Lepton_PID.C input_hipo_files output_file
```
This will create two root trees outFile_Lepton'input_hipo_files'.root and outFile_Pion'input_hipo_files'.root which countain the relevant informations for training

##Training the AI PID
To train the AI PID classifiers, modify the TMVAClassification code and run as:
```
root -l TMVAClassification.C
```

##Plotting variables
You can compare the training variables contained in the root tree by simply running the plotting code in Plot_variables after modifying the path inside the code:
```
root -l Variable_Plots.C
```