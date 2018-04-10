#==========================================#
# split_data.sh formats the data file into
# a more scannable txt file
#
#
#==========================================#


cut -d ‘,’ -f 2 motion_data.csv > acc_x.txt
cut -d ‘,’ -f 3 motion_data.csv > acc_y.txt
cut -d ‘,’ -f 4 motion_data.csv > acc_z.txt
cut -d ‘,’ -f 5 motion_data.csv > gyro_x.txt
cut -d ‘,’ -f 6 motion_data.csv > gyro_y.txt
cut -d ‘,’ -f 7 motion_data.csv > gyro_z.txt
cut -d ‘,’ -f 7 motion_data.csv > mag_x.txt
cut -d ‘,’ -f 7 motion_data.csv > mag_y.txt
cut -d ‘,’ -f 7 motion_data.csv > mag_z.txt
