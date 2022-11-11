#generate PBS for all combined
nodeSquence = [2,4,6,8]
threadSquence = [2,4,6,8,10,12]
fileName = ["combined_omp1_mp1"]
for i in nodeSquence:
    for j in threadSquence:
        template = '''#PBS -N cb-{EXEC}-p{NP}-t{NT}
        #PBS -o results/cb-{EXEC}-p{NP}-t{NT}.log
        #PBS -l nodes={NODES}
        #PBS -l pmem=1gb
        #PBS -l walltime=00:30:00
        #PBS -q coc-ice
        #PBS -j oe

        module load gcc/8.3.0 intel/19.0.5 mvapich2/2.3.2

        export OMP_NUM_THREADS={NT}
        export MPI_NUM_PROCS={NP}
        export MPV_CPU_BINDING_POLICY=hybrid
        export OMP_PROC_BIND=true
        export VIADEV_USE_AFFINITY=0
        export MV2_ENABLE_AFFINITY=0
        export MV2_THREADS_PER_PROCESS={NT}

        cd ~/combined
        make clean all

        cat $PBS_NODEFILE | uniq > hostfile

        mpirun -np {NP} -hostfile hostfile ./{EXEC} {NP} {NT} {NR}
        '''

        NP = i
        NT = j
        NR = 100
        EXEC = fileName[0]

        NODES = '+'.join([f'1:ppn={NT}'] * int(NP))

        pbs_file = open(f'cb-{EXEC}-p{NP}-t{NT}.pbs', 'w')
        pbs_file.write(
            template.format(NP=NP, NR=NR, NT=NT, NODES=NODES, EXEC=EXEC)
        )
        pbs_file.close()