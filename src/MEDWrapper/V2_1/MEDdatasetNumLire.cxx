/*************************************************************************
* COPYRIGHT (C) 1999 - 2002  EDF R&D
* THIS LIBRARY IS FREE SOFTWARE; YOU CAN REDISTRIBUTE IT AND/OR MODIFY
* IT UNDER THE TERMS OF THE GNU LESSER GENERAL PUBLIC LICENSE 
* AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION; 
* EITHER VERSION 2.1 OF THE LICENSE, OR (AT YOUR OPTION) ANY LATER VERSION.
*  
* THIS LIBRARY IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT
* WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. SEE THE GNU
* LESSER GENERAL PUBLIC LICENSE FOR MORE DETAILS.
*
* YOU SHOULD HAVE RECEIVED A COPY OF THE GNU LESSER GENERAL PUBLIC LICENSE
* ALONG WITH THIS LIBRARY; IF NOT, WRITE TO THE FREE SOFTWARE FOUNDATION,
* INC., 59 TEMPLE PLACE, SUITE 330, BOSTON, MA 02111-1307 USA
*
*************************************************************************/

#include "med.hxx"
#include "med_outils.hxx"

/*
 * - Nom de la fonction : _MEDdatasetNumLire
 * - Description : lecture d'un dataset tableau numerique
 * - Parametres :
 *     - pere (IN)     : l'ID de l'objet HDF pere ou placer l'attribut
 *     - nom  (IN)     : le nom du dataset
 *     - type (IN)     : type numerique MED
 *     - interlace (IN) : Choix du type d'entrelacement demand� par l'appelant { MED_FULL_INTERLACE(x1,y1,z1,x2,...)) , MED_NO_INTERLACE(x1,x2,y1,y2,z1,z2) }
 *       - nbdim   (IN) : Dimension des �l�ments
 *       - fixdim  (IN) : MED_ALL ou n� de la dimension a enregistrer � partir de 1..oo
 *     - psize     (IN) : Taille du profil � utiliser, MED_NOPF si pas de profil
 *       - pfltab  (IN) : Tableau contenant les n� d�l�ments � traiter (1....oo)
 *       - pflmod  (IN) : PARAMETRE A AJOUTER : Indique comment lire les informations en m�moire { MED_COMPACT, MED_GLOBALE }. 
 *       - ngauss  (IN) : Nombre de points de GAUSS par �l�ment
 *     - val  (OUT)    : valeurs du tableau
 * - Resultat : 0 en cas de succes, -1 sinon
 *  Equivalent � l'ancienne routine si .....,MED_NO_INTERLACE,1,MED_ALL,MED_NOPF,0,1 (peu importe),....
 */ 

namespace med_2_1{

med_err 
_MEDdatasetNumLire(med_idt pere,char *nom,med_type_champ type,
		   med_mode_switch interlace, med_size nbdim, med_size fixdim, 
		   med_size psize, med_ssize * pfltab, med_int ngauss,
		   unsigned char *val)
{
  med_idt    dataset, dataspace = 0, memspace = 0;
  med_ssize  start_mem[1],start_data[1],*pflmem=0,*pfldsk=0;
  med_size   stride[1],count[1],pcount[1],size[1],pflsize[1];
  med_err    ret;
  int        i,j,index,type_hdf;
  hid_t      datatype;
  size_t     typesize;
  int        dim, firstdim, dimutil, lastdim;
  med_mode_profil pflmod;

  /* Verify fixdim is between [0, nbdim] ( 0 is MED_ALL ) */
  if ( ( fixdim < 0 ) || ( fixdim > nbdim ) ) 
    return -1;
 
  /* block pflmod to MED_COMPACT (until med2.2) */
  pflmod = MED_COMPACT;

  switch(type)
    {
    case MED_REEL64 :
      /* 1) IA32 is LE but due to an (?HDF convertion BUG?) when using H5T_NATIVE_DOUBLE/MED_REEL64? under PCLINUX
	 the file read under SGI is incorrect
	 2) Compaq OSF/1 is LE, since we force SGI64,SUN4SOL2,HP to write double in LE even if they are BE, mips OSF/1 must be BE
	 REM  : Be careful of compatibility between MED files when changing this (med2.2)                    */
#if defined(PCLINUX) || defined(OSF1)
      type_hdf = H5T_IEEE_F64BE;
#else 
      type_hdf = H5T_IEEE_F64LE;
#endif
      break;

    case MED_INT32 :
      type_hdf = H5T_NATIVE_INT;
      break;

    case MED_INT64 :
      type_hdf = H5T_NATIVE_LONG;
      break;

    default :
      return -1;
    }

  /* Ouverture du Dataset � lire */
  if ((dataset = H5Dopen(pere,nom)) < 0)
    return -1;

  /* Interrogation de la taille du dataset */
  if ( (datatype  = H5Dget_type(dataset )) < 0) return -1;
  if ( (typesize  = H5Tget_size(datatype)) < 0) return -1;
  size[0] = H5Dget_storage_size(dataset) / typesize; 
  if ( H5Tclose(datatype) < 0) return -1;

  /* Create dataspace */
  if ((dataspace = H5Screate_simple(1,size,NULL)) < 0)
    return -1;
  
  switch(interlace)
    {
    case MED_FULL_INTERLACE :

      /*Initialisation des indices de boucle du traitement de l'entrelacement en fonction de la dimension fixee*/
      if ( fixdim != MED_ALL) 
	{ 
	  firstdim = fixdim-1;
	  lastdim  = fixdim;
	  dimutil  = 1;
	} else	{
	  firstdim = 0;
	  lastdim = nbdim;
	  dimutil  = nbdim; 
	}

      count [0] = (*size)/(nbdim);
      

      /*rem: Pas de v�rification de l'assertion (*size)=n*nbdim */
      if ( psize == MED_NOPF ) {  

      /* Creation d'un data space m�moire de dimension 1, de longeur size, et de longeur maxi size */
      if ( (memspace = H5Screate_simple (1, size, NULL)) <0)
	return -1;

	stride[0] = nbdim;  

	for (dim=firstdim; dim < lastdim; dim++) {
	  	  
	  start_mem[0] = dim;
	  if ( (ret = H5Sselect_hyperslab (memspace, H5S_SELECT_SET, start_mem, stride, 
					   count, NULL)) <0)
	    return -1; 
	  
	  start_data[0] = dim*count[0];
	  if ( (ret = H5Sselect_hyperslab (dataspace, H5S_SELECT_SET, start_data, NULL, 
					   count, NULL)) <0)
	    return -1; 
	  
	  if ((ret = H5Dread(dataset,type_hdf,memspace,dataspace,
			     H5P_DEFAULT, val)) < 0)
	    return -1;
	}
	
      } else {

	pflsize [0] = psize*ngauss*nbdim;
	pcount  [0] = psize*ngauss*dimutil;
	pflmem     = (med_ssize *) malloc (sizeof(med_ssize)*pcount[0]);
	pfldsk     = (med_ssize *) malloc (sizeof(med_ssize)*pcount[0]);
	
	switch(pflmod)
	  { /* switch pflmod pour FULL_INTERLACE*/
	  case MED_GLOBALE :

	    /* Creation d'un data space m�moire de dimension 1, de longeur size, et de longeur maxi size */
	    if ( (memspace = H5Screate_simple (1, size, NULL)) <0)
	      return -1;

	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye les �lements du profil */
		for (j=0; j < ngauss; j++) {         
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
		  pflmem[index] = (pfltab[i]-1)*ngauss*nbdim + j*nbdim+dim;
		  pfldsk[index] = dim*count[0] + (pfltab[i]-1)*ngauss+j;	     
		}
	    }
	    
	    if ( (ret = H5Sselect_elements(memspace ,H5S_SELECT_SET, pcount[0], (const hssize_t **) pflmem ) ) <0) 
	      return -1; 
	    
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET, pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1; 
	    
	    break;
	
	  case MED_COMPACT :
	
	    /* Creation d'un data space m�moire de dimension 1, de la longeur du profil          */
	    /* La dimension utilis�e est ici nbdim, m�me pour un profil compact on suppose       */
	    /*  que l'utilisateur a toutes les coordon�es stock�es, m�me si il en demande qu'une */ 
	    
	    if ( (memspace = H5Screate_simple (1, pflsize, NULL)) <0)
	      return -1;
	    
	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye les �lements du profil */
		for (j=0; j < ngauss; j++) {         
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
		  pflmem[index] = i*ngauss*nbdim + j*nbdim+dim;
		  pfldsk[index] = dim*count[0] + (pfltab[i]-1)*ngauss+j;	     
		}	      
	    }
	    
	    if ( (ret = H5Sselect_elements(memspace ,H5S_SELECT_SET, pcount[0], (const hssize_t **) pflmem ) ) <0) 
	      return -1; 
	    
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET, pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1; 
	    
	    break;

	  default :
	    return -1; 
	  }
	
	if ((ret = H5Dread(dataset,type_hdf,memspace,dataspace,H5P_DEFAULT, val)) < 0)
	  return -1;
	
	free(pflmem);
	free(pfldsk);
      }
      
      break;
      
    case MED_NO_INTERLACE :

      /*Initialisation des indices de boucle du traitement de l'entrelacement en fonction de la dimension fixee*/

      count[0] = (*size)/nbdim;
      
      if ( psize == MED_NOPF ) {  
	
	if ( fixdim != MED_ALL) 
	  start_data[0] = (fixdim-1)*count[0];
	else {
	  count[0] = *size;
	  start_data[0] =  0;
	};
	
	if ( (ret = H5Sselect_hyperslab (dataspace, H5S_SELECT_SET, start_data, NULL, 
					 count, NULL)) <0)
	  return -1; 
	
	if ((ret = H5Dread(dataset,type_hdf,dataspace,dataspace,
			   H5P_DEFAULT, val)) < 0)
	  return -1;
	
      } else {

	if ( fixdim != MED_ALL) 
	  { 
	    firstdim = fixdim-1;
	    lastdim  = fixdim;
	    dimutil  = 1;
	  } else	{
	    firstdim = 0;
	    lastdim  = nbdim;
	    dimutil  = nbdim; 
	  }

	pflsize [0] = psize*ngauss*nbdim;	
  	pcount  [0] = psize*ngauss*dimutil; /* nom pas tr�s coherent avec count !!! A revoir */	
	pfldsk      = (med_ssize *) malloc(sizeof(med_ssize)*pcount[0]);
	
	switch(pflmod)
	  { /*switch plfmod pour NO_INTERLACE */
	  case MED_GLOBALE :
	    
	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye le nbre d'�lements du profil                */
		for (j=0; j < ngauss; j++) { 
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
		  pfldsk[index] = dim*count[0]+(pfltab[i]-1)*ngauss+j;	    
		}
	    }
	    
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET,pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1;
	    
	    if ((ret = H5Dread(dataset,type_hdf,dataspace,dataspace,H5P_DEFAULT, val)) < 0)
	      return -1;
	      
	    break;
	    
	  case MED_COMPACT :
	    
	    /* Creation d'un data space m�moire de dimension 1, de la longeur du profil          */
	    /* La dimension utilis�e est ici nbdim, m�me pour un profil compact on suppose       */
	    /*  que l'utilisateur a toutes les coordon�es stock�es, m�me si il en demande qu'une */ 

	    if ( (memspace = H5Screate_simple (1, pflsize, NULL)) <0)
	      return -1;

	    pflmem     = (med_ssize *) malloc (sizeof(med_ssize)*pcount[0]);
	    
	    /* Le profil COMPACT est contig�e, mais il est possible que l'on selectionne uniquemenent une dimension*/

	    for (dim=firstdim; dim < lastdim; dim++) {
	      
	      for (i=0; i < psize; i++)              /* i balaye le nbre d'�lements du profil                */
		for (j=0; j < ngauss; j++) {
		  index = i*ngauss+j + (dim-firstdim)*(psize*ngauss);
	          pflmem[index] = dim*(psize*ngauss) + (pfltab[i]-1)*ngauss+j;
		  pfldsk[index] = dim*count[0]  + (pfltab[i]-1)*ngauss+j;	    
		}
	    }
	    
	    if ( (ret = H5Sselect_elements(memspace ,H5S_SELECT_SET, pcount[0], (const hssize_t **) pflmem ) ) <0) 
	      return -1; 
	    
	    if ( (ret = H5Sselect_elements(dataspace,H5S_SELECT_SET,pcount[0], (const hssize_t **) pfldsk ) ) <0) 
	      return -1;	  
	    
	    if ((ret = H5Dread(dataset,type_hdf,memspace,dataspace,H5P_DEFAULT, val)) < 0)
	      return -1;
	    
	    break;
	    
	  default :
	    return -1;	    
	    
	  }
	
	free(pfldsk);
	
      };
      
      break;
      
    default :
      return -1;
    }
  
  

  if (memspace) 
    if ((ret = H5Sclose(memspace)) < 0)
      return -1;

  if ((ret = H5Sclose(dataspace)) < 0)
    return -1;
  
  if ((ret = H5Dclose(dataset)) < 0)
    return -1;      

  return 0;
}

}
