/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2014 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the ZIB Academic License.         */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License.             */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file
 * @brief  Presolver
 * @author Rodolfo Carvajal
 *
 */

/* standard library includes */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

/* scip includes */
#include "scip/scip.h"
#include "scip/scipdefplugins.h"

/* user defined includes */

/* namespace usage */
using namespace std;

//------------------------------------------------------------

int main(int argc, char** argv)
{

    /* problem information */
    int numberVars;

   SCIP* scip = NULL;

   if ( argc != 5)
   {
       cerr << "Wrong number of arguments :(" << endl;
      cerr << "Usage: presolver transfile logfile solfile datafile" 
          << endl << endl;
      cerr << "Args:" << endl;
      cerr << "  transfile: Name of output file for transformed problem\
          (include extension)."<< endl;
      cerr << "  solfile: Solution file for computing opt value in trans mode"
          <<endl;
      cerr << "  logfile: Name of file where to *append* information."<< endl;
      return 1;
   }
   
   string transfile_name = argv[1];
   string logfile_name = argv[3];
   string solfile_name = argv[2];
   ofstream logfile;
   logfile.open(logfile_name.c_str(), ios::out | ios::app);

   /**************
    * Setup SCIP *
    **************/

   /* initialize SCIP environment */
   SCIP_CALL( SCIPcreate(&scip) );

   /***********************
    * Version information *
    ***********************/

   SCIPprintVersion(scip, NULL);
   SCIPinfoMessage(scip, NULL, "\n");

   /* include default plugins*/
   SCIP_CALL( SCIPincludeDefaultPlugins(scip) );

   /* read problem */
   SCIP_CALL( SCIPreadProb(scip, argv[argc-1], NULL) );


   /*************
    *  Solve    *
    *************/
   SCIP_CALL( SCIPreadSol(scip, solfile_name.c_str()) );

   SCIP_CALL( SCIPpresolve(scip) );

   SCIP_CALL( SCIPwriteTransProblem(scip, transfile_name.c_str(), NULL, 1) );

   SCIP_SOL* bestsol = SCIPgetBestSol(scip);
   double transObjVal = SCIPgetSolTransObj(scip, bestsol);

   logfile << argv[argc-1] << " " << transObjVal 
       << " " << SCIPgetNOrigConss(scip)
       << " " << SCIPgetNConss(scip)
       << " " << SCIPgetNOrigVars(scip)
       << " " << SCIPgetNVars(scip) 
       << " " << SCIPgetNOrigContVars(scip)
       << " " << SCIPgetNContVars(scip) 
       << " " << SCIPgetNOrigBinVars(scip)
       << " " << SCIPgetNBinVars(scip) 
       << " " << SCIPgetNOrigIntVars(scip)
       << " " << SCIPgetNIntVars(scip) 
       << " " << SCIPgetPresolvingTime(scip) << endl;
   logfile.close();

   /********************
    * Deinitialization *
    ********************/

   SCIP_CALL( SCIPfree(&scip) );

   BMScheckEmptyMemory();

   return 0;
}

