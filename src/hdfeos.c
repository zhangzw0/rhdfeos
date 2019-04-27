#include <Rdefines.h>
#include <R_ext/Rdynload.h>

#include <hdf.h>
#include <HdfEosDef.h>

void r_he2_open(char **filename, int *hemode, int *fid);
void r_he2_close(int *fid);

void r_he2_inqdims(int *fid, char **swathname);
void r_he2_inqdimmaps(int *fid, char **swathname);
void r_he2_inqdatafields(int *fid, char **swathname);
void r_he2_inqgeolocfields(int *fid, char **swathname);
void r_he2_fieldinfo(int *fid, char **swathname, char **fieldname);

/* .C calls */
static const R_CMethodDef cMethods[] = {
  {"r_he2_open", (DL_FUNC) &r_he2_open, 3},
  {"r_he2_close", (DL_FUNC) &r_he2_close, 1},
  {"r_he2_inqdims", (DL_FUNC) &r_he2_inqdims, 2},
  {"r_he2_inqdimmaps", (DL_FUNC) &r_he2_inqdimmaps, 2},
  {"r_he2_inqdatafields", (DL_FUNC) &r_he2_inqdatafields, 2},
  {"r_he2_inqgeolocfields", (DL_FUNC) &r_he2_inqgeolocfields, 2},
  {"r_he2_fieldinfo", (DL_FUNC) &r_he2_fieldinfo, 3}
};

/* Register C routines for R */
void R_init_rhdfeos(DllInfo *info) {
  R_registerRoutines(info, cMethods, NULL, NULL, NULL);
  R_useDynamicSymbols(info, FALSE);
}

void r_he2_open(char **filename, int *hemode, int *fid) {
  int he_mode;
  if(*hemode == 1)
    he_mode = DFACC_READ;
  else if(*hemode == 2)
    he_mode = DFACC_WRITE;
  else if(*hemode == 3)
    he_mode = DFACC_RDWR;
  else {
    Rprintf("Error in r_he_open: unknown access mode specified. Muse be 1 (read), 2 (write) or 3 (read, write)");
    *fid = -1;
    return;
  }

  int32 nswath, strbufsize;
  char *swathlist;
  *fid = SWopen(R_ExpandFileName(filename[0]), he_mode);
  nswath = SWinqswath(R_ExpandFileName(filename[0]), NULL, &strbufsize);
  swathlist = (char *) calloc(strbufsize + 1, 1);
  nswath = SWinqswath(R_ExpandFileName(filename[0]), swathlist, &strbufsize);

  if (*fid == -1) {
    Rprintf("Cannot open the grid file!");
  } else {
    Rprintf("File open!\n");
    Rprintf("File has %d swaths: %s\n", nswath, swathlist);
    free(swathlist);
  }
}
void r_he2_close(int *fid) {
  intn status;
  status = SWclose(*fid);
  if (status == -1) {
    Rprintf("File cannot be closed!");
  } else {
    Rprintf("File closed");
  }

}

/* Inquire Dimensions */
void r_he2_inqdims(int *fid, char **swathname) {
  int32 swid;
  int32 ndims;
  int32 strbufsize;
  int32 *dims;
  char *dimname;

  swid = SWattach(*fid, *swathname);
  ndims = SWnentries(swid, HDFE_NENTDIM, &strbufsize);
  dims = (int32 *) calloc(ndims, 4);
  dimname = (char *) calloc(strbufsize + 1, 1);
  ndims = SWinqdims(swid, dimname, dims);
  // print dimensions
  Rprintf("dimensions: %s\n", dimname);
  for (int i=0; i < ndims; i++)
    Rprintf("dim size: %d\n", dims[i]);
  free(dims);
  free(dimname);

  int status;
  status = SWdetach(swid);
}
/* Inquire Dimension Mappings */
void r_he2_inqdimmaps(int *fid, char **swathname) {
  int32 swid;
  int32 nmaps;
  int32 strbufsize;
  int32 *off, *inc;
  char *dimmap;

  swid = SWattach(*fid, *swathname);
  nmaps = SWnentries(swid, HDFE_NENTMAP, &strbufsize);
  off = (int32 *) calloc(nmaps, 4);
  inc = (int32 *) calloc(nmaps, 4);
  dimmap = (char *) calloc(strbufsize + 1, 1);
  nmaps = SWinqmaps(swid, dimmap, off, inc);
  if (nmaps == -1){
    Rprintf("Error: cannot get dimmaps");
    return;
  }
  // print dimension maps
  Rprintf("Number of Dimmap: %d\n", nmaps);
  Rprintf("Dimension map: %s\n", dimmap);
  for (int i=0; i < nmaps; i++)
    Rprintf("offset and increment: %d and %d\n", off[i], inc[i]);
  free(off);
  free(inc);
  free(dimmap);

  int status;
  status = SWdetach(swid);
}
/* Inquire Data fields */
void r_he2_inqdatafields(int *fid, char **swathname) {
  int32 swid;
  int32 nflds;
  int32 strbufsize;
  int32 *rank, *ntype;
  char *fieldlist;

  swid = SWattach(*fid, *swathname);
  nflds = SWnentries(swid, HDFE_NENTDFLD, &strbufsize);
  rank = (int32 *) calloc(nflds, 4);
  ntype = (int32 *) calloc(nflds, 4);
  fieldlist = (char *) calloc(strbufsize + 1, 1);
  nflds = SWinqdatafields(swid, fieldlist, rank, ntype);
  // print data fields
  Rprintf("Number of datafields: %d\n", nflds);
  Rprintf("data fields: %s\n", fieldlist);
  for (int i=0; i < nflds; i++)
    Rprintf("rank and type: %d and %d\n", rank[i], ntype[i]);
  free(rank);
  free(ntype);
  free(fieldlist);

  int status;
  status = SWdetach(swid);
}
/* Inquire Geo fields */
void r_he2_inqgeolocfields(int *fid, char **swathname) {
  int32 swid;
  int32 nflds;
  int32 strbufsize;
  int32 *rank, *ntype;
  char *fieldlist;

  swid = SWattach(*fid, *swathname);
  nflds = SWnentries(swid, HDFE_NENTGFLD, &strbufsize);
  rank = (int32 *) calloc(nflds, 4);
  ntype = (int32 *) calloc(nflds, 4);
  fieldlist = (char *) calloc(strbufsize + 1, 1);
  nflds = SWinqgeofields(swid, fieldlist, rank, ntype);
  // print geo fields
  Rprintf("Number of geofields: %d\n", nflds);
  Rprintf("geofields fields: %s\n", fieldlist);
  for (int i=0; i < nflds; i++)
    Rprintf("rank and type: %d and %d\n", rank[i], ntype[i]);
  free(rank);
  free(ntype);
  free(fieldlist);

  int status;
  status = SWdetach(swid);
}


// get info on dims

// get info on fields
void r_he2_fieldinfo(int *fid, char **swathname, char **fieldname) {
  int32 swid, status;
  int32 rk, nt;
  char dimlist[80];
  int32 dim[8];

  swid = SWattach(*fid, *swathname);
  status = SWfieldinfo(swid, *fieldname, &rk, dim, &nt, dimlist);
  if (status == -1)
  {
    Rprintf("Cannot get field info for field\n");
    return;
  }
  Rprintf("Longitude Rank: %d\n", rk);
  Rprintf("Longitude NumberType: %d\n", nt);
  Rprintf("Longitude Dimension List: %s\n", dimlist);
  for (int i=0; i<rk; i++)
    Rprintf("Dimension %d: %d\n",i+1, dim[i]);

  status = SWdetach(swid);
}
