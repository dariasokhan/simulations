/**************************************************/
/* Macro to create a bunch of Auger submission    */
/* scripts. Takes the number of jobs to submit as */
/* option. To run:                                */
/*                                                */
/*       root -l                                  */
/*        [] .L create_job_scripts.C              */
/*        [] create_job_scripts(N)                */ 
/*                                                */
/*  where N is the number of jobs you want.       */
/*  Will create a submission script per job,      */
/*  all with a .sub extension.                    */
/*                                                */
/*  Next, move them to a separate directory       */
/*  (or maybe you already created them there),    */
/*  and run the submission script on that dir:    */
/*                                                */
/*  ./submit_jobs.sh scripts_dir                  */ 
/*                                                */
/*  This will submit everything in the            */
/*  scripts_dir, one by one, slowly, so that      */
/*  the farm doesn't get jammed, and save         */
/*  confirmation of the submission with an        */
/*  _auger extension. The script will only submit */ 
/*  jobs for which no "*_auger" file exists in    */
/*  the directory, to avoid duplicate             */
/*  submissions.                                  */
/*                                                */
/*  Daria Sokhan                                  */
/**************************************************/

// Possibly the most convoluted and moronic way of doing this, and I would have spent the time better to learn shell scripting, but at least this bastard works.

char command1[800];
char command2[800];
char command3[800];
char command4[800];
char command5[800];
char command6[800];
char command7[2500];
char command8[800];
char command9[800];
char command10[800];
char command11[800];
char command12[1000];
char command13[800];
char command14[800];

char subname[500];
char ananame[2000];
char outfile[500];
char infile[500];

void create_job_scripts(int Njobs){
  
  for (int fileno = 0; fileno<(Njobs); fileno++){
    
    sprintf(subname,"/u/home/daria/dvcs_sim/dvcsgen_%d.sub",fileno);  // submission script file
    
    sprintf(ananame,"/w/work/clas12/daria/dvcsRC/dvcsgen --trig 10000 --nmax 10000 --beam 10.604 --x 0.05 0.85 --q2 1.6 14 --t 0 1.79 --gpd 101 --y 0.15 0.9 --w 3.61 --zpos -3 --zwidth 5 --raster 0.025 --vv2cut 0.3 --delta 0.01 --radgen --writef 2");   // generator executable

    // Notes specific to dvcsgen:
    // to save all events with their weight (generate flat), add the --weight option
    // to run for inbending specifically (if accept/reject is used), change q2 range to start at 1.6
    // to use the born cross-section for the accept/reject method, add the --radstable flag
    
    sprintf(outfile,"/volatile/clas12/daria/gen/dvcsgen_Q2min1.6_%d.txt",fileno);    // output file and location
    
    sprintf(infile,"/w/work/clas12/daria/dvcsRC/_CFF_grid_with_Dterm.data");    // input files have to be copied to same location
    
    // lines to be printed into the submission script file:
    sprintf(command1,"echo 'JOBNAME: dvcsgen_rad_%d' > %s",fileno,subname);
    sprintf(command2,"echo 'PROJECT: clas12' >> %s",subname);
    sprintf(command3,"echo 'MAIL: daria@jlab.org' >> %s",subname);
    sprintf(command4,"echo 'DISK_SPACE: 12 GB' >> %s",subname);
    sprintf(command5,"echo 'MEMORY: 1200 MB' >> %s",subname);
    sprintf(command6,"echo 'TIME: 360' >> %s",subname);
    sprintf(command7,"echo 'COMMAND: source /group/clas12/packages/setup.csh; module load clas12/pro; %s' >> %s",ananame,subname);
    sprintf(command8,"echo 'OS: centos77' >> %s",subname);
    sprintf(command9,"echo 'TRACK: simulation' >> %s",subname);
    sprintf(command10,"echo 'INPUT_FILES:' >> %s",subname);
    sprintf(command11,"ls %s >> %s",infile,subname);
    sprintf(command12,"echo 'SINGLE_JOB: true' >> %s",subname);
    sprintf(command13,"echo 'OUTPUT_DATA: dvcsgen1.dat' >> %s",subname);
    sprintf(command14,"echo 'OUTPUT_TEMPLATE: %s' >> %s",outfile,subname);
    
    
    gSystem->Exec(command1);
    gSystem->Exec(command2);
    gSystem->Exec(command3);
    gSystem->Exec(command4);
    gSystem->Exec(command5);
    gSystem->Exec(command6);
    gSystem->Exec(command7);
    gSystem->Exec(command8);
    gSystem->Exec(command9);
    gSystem->Exec(command10);
    gSystem->Exec(command11);
    gSystem->Exec(command12);
    gSystem->Exec(command13);
    gSystem->Exec(command14);    

  }
  
}
