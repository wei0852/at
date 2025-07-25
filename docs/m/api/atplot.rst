.. _atplot_module:

atplot
======

.. py:module:: atplot

   ATPLOT

.. toctree::
   :hidden:

   atplot.plotfunctions

.. rubric:: Modules


.. list-table::

   * - :ref:`plotfunctions_module`
     - PLOTFUNCTIONS

.. rubric:: Functions


.. list-table::

   * - :func:`atbaseplot`
     - Plots data generated by a user-supplied function
   * - :func:`atplot`
     - Plots optical functions
   * - :func:`atplotsyn`
     - Helper function for ATPLOT
   * - :func:`atreforbit`
     - Keep track of the nominal reference orbit through displaced elements

.. py:function:: atbaseplot(ring)

   | Plots data generated by a user-supplied function
   
   | **atbaseplot**             Plots THERING in the current axes
   
   | **atbaseplot(ring)**       Plots the lattice specified by RING
   
   | **atbaseplot(ring,dpp)   plots at momentum deviation dpp (default: 0)**
   
   | **atbaseplot(...,[smin smax])**  Zoom on the specified range
   
   | **atbaseplot(...,@plotfunction,plotargs)**
   |    PLOTFUNCTION: User supplied function providing the values to be plotted
   |    PLOTARGS:     Optional cell array of arguments to PLOTFUNCTION
   |    PLOTFUNCTION is called as:
   |    [S,PLOTDATA]=PLOTFUNCTION(RING,DPP,PLOTARGS{:})
   
   |        S:        longitudinal position, length(ring)+1 x 1
   |        PLOTDATA: structure array,
   |          PLOTDATA(1) describes data for the left (main) axis
   |            PLOTDATA(1).VALUES: data to be plotted, length(ring)+1 x nbcurve
   |            PLOTDATA(1).LABELS: curve labels, cell array, 1 x nbcurve
   |            PLOTDATA(1).AXISLABEL: string
   |          PLOTDATA(2) optional, describes data for the right (secondary) axis
   
   | **atbaseplot(...,'optionname',optionvalue,...)** Available options:
   |    'synopt',true|false         Plots the lattice elements
   |    'labels',REFPTS             Display the names of selected element names
   |    'index',REFPTS             Display the index of selected element names
   |    'leftargs',{properties}     properties set on the left axis
   |    'rightargs',{properties}    properties set on the right axis
   |    'KeepAxis'                  flag to keep R1,R2,T1,T2 at each slice in
   |                                detailed plots (mandatory with vert. bend).
   
   | **atbaseplot(ax,...)**     Plots in the axes specified by AX. AX can precede
   |                        any previous argument combination
   
   | **curve=atbaseplot(...)**  Returns handles to some objects:
   |    CURVE.PERIODICIY ring periodicity
   |    CURVE.LENGTH    structure length
   |    CURVE.DPP       deltap/p
   |    CURVE.LEFT      Handles to the left axis plots
   |    CURVE.RIGHT     Handles to the right axis plots
   |    CURVE.LATTICE   Handles to the Element patches: structure with fields
   |           Dipole,Quadrupole,Sextupole,Multipole,BPM,Label
   

.. py:function:: atplot(ring)

   | Plots optical functions
   
   | **atplot**                 Plots THERING in the current axes
   
   | **atplot(ring)**           Plots the lattice specified by RING
   
   | **atplot(ax,ring)**        Plots in the axes specified by AX
   
   | **atplot(ax,ring,dpp)**    Plots at momentum deviation DPP
   
   | **atplot(...,[smin smax])**  Zoom on the specified range
   
   | **atplot(...,'optionname',optionvalue,...)** Available options:
   |    'twiss_in',structure of optics   transferline optics
   |                (ex: [optics_struct,~,~]=atlinopt(ring,0,1);
   |               **atplot(ring,[0 10],@plbeamsize,'twiss_in',optics_struct);)**
   |    'comment',true|false        Prints lattice information (default:true)
   |    'synopt',true|false         Plots the lattice elements
   |    'labels',REFPTS             Display the names of selected element names
   |    'index',REFPTS             Display the index of selected element names
   |    'leftargs',{properties}     properties set on the left axis
   |    'rightargs',{properties}    properties set on the right axis
   |    'KeepAxis'                  flag to keep R1,R2,T1,T2 at each slice in
   |                                detailed plots (mandatory with vert. bend).
   
   | **atplot(...,@plotfunction,args...)**
   | 	Allows for a user supplied function providing the values to be plotted
   |    PLOTFUNCTION must be of form:
   |    PLOTDATA=PLOTFUNCTION(LINDATA,RING,DPP,args...)
   
   |        PLOTDATA: structure array,
   |          PLOTDATA(1) describes data for the left (main) axis
   |            PLOTDATA(1).VALUES: data to be plotted, length(ring)+1 x nbcurve
   |            PLOTDATA(1).LABELS: curve labels, cell array, 1 x nbcurve
   |            PLOTDATA(1).AXISLABEL: string
   |          PLOTDATA(2) optional, describes data for the right (secondary) axis
   
   |    The default function displayed below as an example plots beta functions
   | 	and dispersion
   
   |  function plotdata=defaultplot(lindata,ring,dpp,varargin)
   |  beta=cat(1,lindata.beta);                     % left axis
   |  plotdata(1).values=beta;
   |  plotdata(1).labels={'\beta_x','\beta_z'};
   |  plotdata(1).axislabel='\beta [m]';
   |  dispersion=cat(2,lindata.Dispersion)';        % right axis
   |  plotdata(2).values=dispersion(:,1);
   |  plotdata(2).labels={'\eta_x'};
   |  plotdata(2).axislabel='dispersion [m]';
   |  end
   
   | **curve=atplot(...)** Returns handles to some objects:
   |    CURVE.PERIODICIY ring periodicity
   |    CURVE.LENGTH    structure length
   |    CURVE.DPP       deltap/p
   |    CURVE.LEFT      Handles to the left axis plots
   |    CURVE.RIGHT     Handles to the right axis plots
   |    CURVE.LATTICE   Handles to the Element patches: structure with fields
   |           Dipole,Quadrupole,Sextupole,Multipole,BPM,Label
   |    CURVE.COMMENT   Handles to the Comment text
   
   
   | **atplot** calls the more general ATBASEPLOT function, which uses a slightly
   | different syntax.
   
   | See also :func:`atbaseplot`

.. py:function:: atplotsyn(ax,ring)

   | Helper function for ATPLOT
   
   | **patches=atplotsyn(ax,ring)** Plots the magnetic elements found in RING

.. py:function:: atreforbit(ring)

   | Keep track of the nominal reference orbit through displaced elements
   
   | Element displacement vectors T1 and T2 are often used to introduce positioning
   | errors of elements. When plotting the resulting closed orbit or trajectories,
   | it is useful to plot positions with respect to the theoretical reference rather
   | than to the displaced reference. This function generates reference coordinates
   | xref and zref so that:
   
   | - R(1) and R(3) are the particles horizontal and vertical positions with
   | respect to the actual (displaced) reference,
   
   | - R(1)+xref and R(3)+zref are the positions with respect to the ideal reference.
   
   | If any of T1 or T2 is part of the design of the ring, the reference
   | translation can be skipped by setting a field 'hideT1' or 'hideT2' on the
   | element.
   
   |   INPUTS
   |   1. ring Ring structure
   
   |   OUTPUTS
   |   1. xref Horizontal reference orbit shift
   |   2. zref Vertical reference orbit shift
   
   |   EXAMPLE
   |   1. **[xref,zref]=atreforbit(ring)**
   
   | See also :func:`atplot`

