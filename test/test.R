library(rhdfeos)
he <- he2_open("data/AMSR_E_L2A_BrightnessTemperatures_V09_200206190029_D.hdf")
he <- he2_open("data/swath.hdf")
he

swath <- "Swath1"
swath <- "High_Res_A_Swath"
he2_inqdims(he, "High_Res_B_Swath")
he2_inqdimmaps(he, "High_Res_B_Swath")
he2_inqdatafields(he, "High_Res_A_Swath")
he2_inqgeolocfields(he, swath)
he2_inqgeolocfields(he, "High_Res_A_Swath")

he2_fieldinfo(he, swathname = swath, fieldname = "Latitude")

he2_close(he)
