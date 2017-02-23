# HighMassCombInputs

# 2D templates:
- In the parent directory of HighMassCombInputs, please also check out and compile the smoothing package from Jean-Baptiste:
https://github.com/jbsauvan/TemplateBuilder.git

- copy all the smoothing configs from 
https://github.com/CandiceYou/HighMassCombInputs/tree/2l2q/smConfig
to /TemplateBuilder/run/

- uncomment the 2D template part in run/run2l2q_all.sh and run this bash script

- output files:

(1) The root file to use in combine, with all 2D templates and systematics:

/2l2q2DTemplate/2l2q_spin0_template.root 

/2l2q2DTemplate/2l2q_spin2_template.root 

(2) The 2D template plots:

/2l2q2DTemplate/*.png

(3) The 1D plots in mass slices, with systematics and data-MC comparisons:

/2l2q2DTemplate/spin0/1DSlices/*.png

/2l2q2DTemplate/spin2/1DSlices/*.png

useful ones are:

 1,sig vs DY bkg: spin0_*_DY_*.png
 
 2,all mass slices together: spin0_*_1D_all.png (this also give you binning information)

