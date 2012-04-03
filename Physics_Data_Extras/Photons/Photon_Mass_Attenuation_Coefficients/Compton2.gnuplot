#!/usr/bin/gnuplot -persist
#
#    
#    	G N U P L O T
#    	Version 4.4 patchlevel 4
#    	last modified November 2011
#    	System: Linux 3.2.11-1-ARCH
#    
#    	Copyright (C) 1986-1993, 1998, 2004, 2007-2011
#    	Thomas Williams, Colin Kelley and many others
#    
#    	gnuplot home:     http://www.gnuplot.info
#    	faq, bugs, etc:   type "help seeking-assistance"
#    	immediate help:   type "help"
#    	plot window:      hit 'h'
# set terminal wxt 0
# set output
unset clip points
set clip one
unset clip two
set bar 1.000000 front
set border 31 front linetype -1 linewidth 1.000
set xdata
set ydata
set zdata
set x2data
set y2data
set timefmt x "%d/%m/%y,%H:%M"
set timefmt y "%d/%m/%y,%H:%M"
set timefmt z "%d/%m/%y,%H:%M"
set timefmt x2 "%d/%m/%y,%H:%M"
set timefmt y2 "%d/%m/%y,%H:%M"
set timefmt cb "%d/%m/%y,%H:%M"
set boxwidth
set style fill  empty border
set style rectangle back fc  lt -3 fillstyle   solid 1.00 border lt -1
set style circle radius graph 0.02, first 0, 0 
set dummy x,y
set format x "% g"
set format y "% g"
set format x2 "% g"
set format y2 "% g"
set format z "% g"
set format cb "% g"
set angles radians
unset grid
set key title ""
set key inside left top vertical Right noreverse enhanced autotitles nobox
set key noinvert samplen 4 spacing 1 width 0 height 0 
set key maxcolumns 0 maxrows 0
unset label
unset arrow
set style increment default
unset style line
unset style arrow
set style histogram clustered gap 2 title  offset character 0, 0, 0
unset logscale
set logscale x 10
set logscale y 10
set offsets 0, 0, 0, 0
set pointsize 1
set pointintervalbox 1
set encoding default
unset polar
unset parametric
unset decimalsign
set view 60, 30, 1, 1
set samples 10000, 10000
set isosamples 10, 10
set surface
unset contour
set clabel '%8.3g'
set mapping cartesian
set datafile separator whitespace
unset hidden3d
set cntrparam order 4
set cntrparam linear
set cntrparam levels auto 5
set cntrparam points 5
set size ratio 0 1,1
set origin 0,0
set style data points
set style function lines
set xzeroaxis linetype -2 linewidth 1.000
set yzeroaxis linetype -2 linewidth 1.000
set zzeroaxis linetype -2 linewidth 1.000
set x2zeroaxis linetype -2 linewidth 1.000
set y2zeroaxis linetype -2 linewidth 1.000
set ticslevel 0.5
set mxtics default
set mytics default
set mztics default
set mx2tics default
set my2tics default
set mcbtics default
set xtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set xtics autofreq  norangelimit
set ytics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set ytics autofreq  norangelimit
set ztics border in scale 1,0.5 nomirror norotate  offset character 0, 0, 0
set ztics autofreq  norangelimit
set nox2tics
set noy2tics
set cbtics border in scale 1,0.5 mirror norotate  offset character 0, 0, 0
set cbtics autofreq  norangelimit
set title "" 
set title  offset character 0, 0, 0 font "" norotate
set timestamp bottom 
set timestamp "" 
set timestamp  offset character 0, 0, 0 font "" norotate
set rrange [ * : * ] noreverse nowriteback  # (currently [8.98847e+307:-8.98847e+307] )
set trange [ * : * ] noreverse nowriteback  # (currently [-5.00000:5.00000] )
set urange [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set vrange [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set xlabel "Energy (MeV)" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set x2label "" 
set x2label  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ * : * ] noreverse nowriteback  # (currently [-3.00000:2.00000] )
set x2range [ * : * ] noreverse nowriteback  # (currently [-3.00000:1.69897] )
set ylabel "Mass Attenuation Coefficient" 
set ylabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set y2label "" 
set y2label  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set yrange [ 0.00100000 : 1.00000 ] noreverse nowriteback
set y2range [ * : * ] noreverse nowriteback  # (currently [-3.00000:0.00000] )
set zlabel "" 
set zlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set zrange [ * : * ] noreverse nowriteback  # (currently [-10.0000:10.0000] )
set cblabel "" 
set cblabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate by -270
set cbrange [ * : * ] noreverse nowriteback  # (currently [8.98847e+307:-8.98847e+307] )
set zero 1e-08
set lmargin  -1
set bmargin  -1
set rmargin  -1
set tmargin  -1
set locale "en_US.UTF-8"
set pm3d explicit at s
set pm3d scansautomatic
set pm3d interpolate 1,1 flush begin noftriangles nohidden3d corners2color mean
set palette positive nops_allcF maxcolors 0 gamma 1.5 color model RGB 
set palette rgbformulae 7, 5, 15
set colorbox default
set colorbox vertical origin screen 0.9, 0.2, 0 size screen 0.05, 0.6, 0 front bdefault
set loadpath 
set fontpath 
set fit noerrorvariables
g(x) = I*(f1(x)**(-1) + f2(x)**(-1) + f3(x)**(-1) + f4(x)**(-1))**(-1)
f(x) = ((x**(-A))/C +  (x**(-B))/C) * (1.0-exp(-x*D))/(1.0+exp(-x*D))
f1(x) = (A)*(x**(B))
f2(x) = (C)*(x**(D))
f3(x) = (E)*(x**(F))
f4(x) = (G)*(x**(H))
h(x) = (1.0/D)*(x**(log(A*x*x+B*x+C)))
GNUTERM = "wxt"
GPFUN_g = "g(x) = I*(f1(x)**(-1) + f2(x)**(-1) + f3(x)**(-1) + f4(x)**(-1))**(-1)"
A = 0.0187388718108881
C = 1600.34495587148
B = -1.15550304864446
D = 2.18651918070461
GPFUN_f = "f(x) = ((x**(-A))/C +  (x**(-B))/C) * (1.0-exp(-x*D))/(1.0+exp(-x*D))"
FIT_CONVERGED = 1
I = 33.4604904190791
GPFUN_h = "h(x) = (1.0/D)*(x**(log(A*x*x+B*x+C)))"
GPFUN_f1 = "f1(x) = (A)*(x**(B))"
GPFUN_f2 = "f2(x) = (C)*(x**(D))"
E = 0.00254634158022194
F = -0.467980119564323
GPFUN_f3 = "f3(x) = (E)*(x**(F))"
G = 0.0334033835752647
H = 0.35737887482621
GPFUN_f4 = "f4(x) = (G)*(x**(H))"
FIT_NDF = 30
FIT_STDFIT = 0.000486023457859152
FIT_WSSR = 7.08656404768102e-06
plot 'Compton' linestyle 2, f1(x), f2(x), f3(x), f4(x), g(x)
#    EOF
