/* CavityPass.c

 * Accelerator Toolbox
 * Revision 3/10/04
 * A.Terebilo
 */
#include "atconstants.h"
#include "atelem.c"

struct elem
{
    double Length;
    double Voltage;
    double Energy;
    double Frequency;
    /* optional fields */
    double TimeLag;
    double PhaseLag;
};

void CavityPass(double *r_in, double le, double nv, double freq, double lag, double philag, int num_particles)
/* le - physical length
 * nv - peak voltage (V) normalized to the design enegy (eV)
 * r is a 6-by-N matrix of initial conditions reshaped into
 * 1-d array of 6*N elements
 */
{	int c, c6;
    double halflength , p_norm, NormL;
    if(le == 0)
    {	for(c = 0;c<num_particles;c++)
        {	c6 = c*6;
            if(!atIsNaN(r_in[c6]))
                r_in[c6+4] += -nv*sin(TWOPI*freq*(r_in[c6+5]-lag)/C0-philag);
        }
    }
    else
    {	halflength = le/2;
        for(c = 0;c<num_particles;c++)
        {	c6 = c*6;
            if(!atIsNaN(r_in[c6]))
            {   p_norm = 1/(1+r_in[c6+4]);
                NormL  = halflength*p_norm;
                /* Prropagate through a drift equal to half cavity length */
                r_in[c6+0]+= NormL*r_in[c6+1];
                r_in[c6+2]+= NormL*r_in[c6+3];
                r_in[c6+5]+= NormL*p_norm*(r_in[c6+1]*r_in[c6+1]+r_in[c6+3]*r_in[c6+3])/2;
                /* Longitudinal momentum kick */
                r_in[c6+4] += -nv*sin(TWOPI*freq*(r_in[c6+5]-lag)/C0-philag);
                p_norm = 1/(1+r_in[c6+4]);
                NormL  = halflength*p_norm;
                /* Prropagate through a drift equal to half cavity length */
                r_in[c6+0]+= NormL*r_in[c6+1];
                r_in[c6+2]+= NormL*r_in[c6+3];
                r_in[c6+5]+= NormL*p_norm*(r_in[c6+1]*r_in[c6+1]+r_in[c6+3]*r_in[c6+3])/2;
            }
        }
    }
}



#if defined(MATLAB_MEX_FILE) || defined(PYAT)
ExportMode struct elem *trackFunction(const atElem *ElemData,struct elem *Elem,
        double *r_in, int num_particles, struct parameters *Param)
{
    double energy = Param->energy;
    if (!Elem) {
        double Length, Voltage, Energy, Frequency, TimeLag, PhaseLag;
        Length=atGetDouble(ElemData,"Length"); check_error();
        Voltage=atGetDouble(ElemData,"Voltage"); check_error();
        Frequency=atGetDouble(ElemData,"Frequency"); check_error();
        /*optional fields*/
        Energy=atGetOptionalDouble(ElemData,"Energy",energy); check_error();
        TimeLag=atGetOptionalDouble(ElemData,"TimeLag",0); check_error();
        PhaseLag=atGetOptionalDouble(ElemData,"PhaseLag",0); check_error();
        Elem = (struct elem*)atMalloc(sizeof(struct elem));
        Elem->Length=Length;
        Elem->Voltage=Voltage;
        Elem->Energy=Energy;
        Elem->Frequency=Frequency;
        Elem->TimeLag=TimeLag;
        Elem->PhaseLag=PhaseLag;
    }
    if (energy == 0.0) energy = Elem->Energy;

    CavityPass(r_in,Elem->Length,Elem->Voltage/energy,Elem->Frequency,
            Elem->TimeLag,Elem->PhaseLag,num_particles);
    return Elem;
}

MODULE_DEF(CavityPass)        /* Dummy module initialisation */

#endif /*defined(MATLAB_MEX_FILE) || defined(PYAT)*/

#if defined(MATLAB_MEX_FILE)

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs >= 2) {
        double rest_energy = 0.0;
        double charge = -1.0;
        double *r_in;
        const mxArray *ElemData = prhs[0];
        int num_particles = mxGetN(prhs[1]);
        double Length=atGetDouble(ElemData,"Length");
        double Voltage=atGetDouble(ElemData,"Voltage");
        double Frequency=atGetDouble(ElemData,"Frequency");
        double Energy=atGetOptionalDouble(ElemData,"Energy",0.0);
        double TimeLag=atGetOptionalDouble(ElemData,"TimeLag",0);
        double PhaseLag=atGetOptionalDouble(ElemData,"PhaseLag",0);
        if (nrhs > 2) atProperties(prhs[2], &Energy, &rest_energy, &charge);

        if (mxGetM(prhs[1]) != 6) mexErrMsgIdAndTxt("AT:WrongArg","Second argument must be a 6 x N matrix");
        /* ALLOCATE memory for the output array of the same size as the input  */
        plhs[0] = mxDuplicateArray(prhs[1]);
        r_in = mxGetDoubles(plhs[0]);

        CavityPass(r_in,Length,Voltage/Energy,Frequency,TimeLag,PhaseLag,num_particles);
    }
    else if (nrhs == 0) {   /* return list of required fields */
        plhs[0] = mxCreateCellMatrix(4,1);
        mxSetCell(plhs[0],0,mxCreateString("Length"));
        mxSetCell(plhs[0],1,mxCreateString("Voltage"));
        mxSetCell(plhs[0],2,mxCreateString("Energy"));
        mxSetCell(plhs[0],3,mxCreateString("Frequency"));
        if (nlhs>1) /* optional fields */
        {   plhs[1] = mxCreateCellMatrix(2,1);
            mxSetCell(plhs[1],0,mxCreateString("TimeLag"));
            mxSetCell(plhs[1],1,mxCreateString("PhaseLag"));
        }
    }
	else {
        mexErrMsgIdAndTxt("AT:WrongArg","Needs 0 or 2 arguments");
	}
}
#endif /* MATLAB_MEX_FILE */
