#!/bin/bash

echo "#!/bin/bash" > $2/startup-run
echo "echo tjurm | sudo -S cpufreq-set -g performance" >> $2/startup-run
# echo "mkdir $1/Mark" >> $2/startup-run
# echo "mkdir $1/gimbal_video" >> $2/startup-run
# echo "mkdir $1/armor_box_photo" >> $2/startup-run
echo "gnome-terminal -- bash -c \"echo tjurm | sudo -S $1/Tools/monitor.sh \\\"$2/run \\\"\"" >> $2/startup-run
chmod +x $2/startup-run
