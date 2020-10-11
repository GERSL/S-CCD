#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <getopt.h>

#include "const.h"
#include "utilities.h"
#include "defines.h"


/*****************************************************************************
  NAME:  write_message

  PURPOSE:  Writes a formatted log message to the specified file handle.

  RETURN VALUE:  None

  NOTES:
      - Log Message Format:
            yyyy-mm-dd HH:mm:ss pid:module [filename]:line message
*****************************************************************************/

void write_message
(
    const char *message, /* I: message to write to the log */
    const char *module,  /* I: module the message is from */
    const char *type,    /* I: type of the error */
    char *file,          /* I: file the message was generated in */
    int line,            /* I: line number in the file where the message was
                               generated */
    FILE *fd             /* I: where to write the log message */
)
{
    time_t current_time;
    struct tm *time_info;
    int year;
    pid_t pid;

    time (&current_time);
    time_info = localtime (&current_time);
    year = time_info->tm_year + 1900;

    pid = getpid ();

    fprintf (fd, "%04d:%02d:%02d %02d:%02d:%02d %d:%s [%s]:%d [%s]:%s\n",
             year,
             time_info->tm_mon,
             time_info->tm_mday,
             time_info->tm_hour,
             time_info->tm_min,
             time_info->tm_sec,
             pid, module, basename (file), line, type, message);
}


/*****************************************************************************
  NAME:  sub_string

  PURPOSE:  To control the specific way in with a string is manipulated.

  RETURN VALUE:  Sub-setted character string

  NOTES:  Probably dangerous.
*****************************************************************************/

char *sub_string         /* explicit control of a substring function  */
(
    const char *source,  /* I: input string                           */
    size_t start,        /* I: index for start of sub string          */
    size_t length        /* I: number of characters to grab           */
)
{
    size_t i;
    char *target;

    target = malloc(length*sizeof(char));

    for(i = 0; i != length; ++i)
    {
        target[i] = source[start + i];
    }
    target[i] = 0;
    return target;
}


/******************************************************************************
MODULE:  quick_sort

PURPOSE:  sort the scene_list & sdate based on yeardoy string

RETURN VALUE: None

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
1/23/2015   Song Guo         Original Development

NOTES:
******************************************************************************/
void quick_sort (int arr[], char *brr[], int crr[], int left, int right)
{
    int index = partition (arr, brr, crr, left, right);

    if (left < index - 1)
    {
        quick_sort (arr, brr, crr, left, index - 1);
    }
    if (index < right)
    {
        quick_sort (arr, brr, crr, index, right);
    }
}

/******************************************************************************
MODULE:  quick_sort_buf

PURPOSE:  sort the point-based buffer data & sdate based on yeardoy string

RETURN VALUE: None

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
8/28/2019   Su Ye            Original Development

NOTES:
******************************************************************************/
void quick_sort_buf (int arr[], short int **brr, short int *fmask_buf, int left, int right)
{
    int index = partition_buf(arr, brr, fmask_buf, left, right);

    if (left < index - 1)
    {
        quick_sort_buf (arr, brr, fmask_buf, left, index - 1);
    }
    if (index < right)
    {
        quick_sort_buf (arr, brr, fmask_buf, index, right);
    }
}

void quick_sort_buf_sensor (int arr[], short int **brr, short int *fmask_buf, short int *sensor, int left, int right)
{
    int index = partition_buf_sensor(arr, brr, fmask_buf, sensor, left, right);

    if (left < index - 1)
    {
        quick_sort_buf_sensor (arr, brr, fmask_buf, sensor, left, index - 1);
    }
    if (index < right)
    {
        quick_sort_buf_sensor (arr, brr, fmask_buf, sensor, index, right);
    }
}

void quick_sort_index (int arr[], int *index,  int left, int right)
{
    int ind = partition_index(arr, index,  left, right);

    if (left < ind - 1)
    {
        quick_sort_index(arr, index,  left, ind - 1);
    }
    if (ind < right)
    {
        quick_sort_index(arr, index,  ind - 1, right);
    }
}

/******************************************************************************
MODULE:  quick_sort_float

PURPOSE:  sort the scene_list based on yeardoy string

RETURN VALUE: None

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
05/27/2019   Su Ye        Original Development

NOTES:
******************************************************************************/
void quick_sort_float(float arr[], int left, int right)
{
    int index = partition_float (arr, left, right);

    if (left < index - 1)
    {
        quick_sort_float (arr, left, index - 1);
    }
    if (index < right)
    {
        quick_sort_float (arr, index, right);
    }
}

/******************************************************************************
MODULE:  quick_sort_float

PURPOSE:  sort the scene_list based on yeardoy string

RETURN VALUE: None

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
05/27/2019   Su Ye        Original Development

NOTES:
******************************************************************************/
void quick_sort_double(double arr[], int left, int right)
{
    int index = partition_double (arr, left, right);

    if (left < index - 1)
    {
        quick_sort_double (arr, left, index - 1);
    }
    if (index < right)
    {
        quick_sort_double (arr, index, right);
    }
}

/******************************************************************************
MODULE:  partition

PURPOSE:  partition used for the quick_sort routine

RETURN VALUE:
Type = int
Value           Description
-----           -----------
i               partitioned value

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
1/23/2015   Song Guo         Original Development
01/02/2020   Su Ye        fix bug of i == j

NOTES:
******************************************************************************/
int partition (int arr[], char *brr[], int crr[], int left, int right)
{
    int i = left, j = right;
    int tmp, tmp2;
    char temp[MAX_STR_LEN];
    int pivot = arr[(left + right) / 2];

    while (i <= j)
    {
        //printf("arr[i] = %d\n", arr[i]);
        //printf("arr[j] = %d\n", arr[j]);
        //printf("pivot = %d\n", pivot);
        while (arr[i] < pivot)
        {
            i++;
        }
        while (arr[j] > pivot)
        {
            j--;
        }
        //printf("i = %d\n", i);
        //printf("j = %d\n", j);
        if (i < j)
        {
            tmp = arr[i];
            strcpy(&temp[0], brr[i]);
            //printf("brr[i] = %s\n", brr[i]);
            //printf("brr[j] = %s\n", brr[j]);
            //printf("crr[i] = %d\n", crr[i]);
            //printf("crr[j] = %d\n", crr[j]);
            tmp2 = crr[i];
            arr[i] = arr[j];
            //printf("step 1 finished \n");
            strcpy(brr[i], brr[j]);
            //printf("step 2 finished \n");
            crr[i] = crr[j];

            arr[j] = tmp;
            strcpy(brr[j],&temp[0]);
            //printf("step 3 finished \n");
            crr[j] = tmp2;   
            i++;
            j--;
        }
        else if (i == j)
        {
            i++;
            j--;
        }
    }

    return i;
}

/******************************************************************************
MODULE:  partition_buf

PURPOSE:  partition buffer used for the quick_sort routine

RETURN VALUE:
Type = int
Value           Description
-----           -----------
i               partitioned value

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
8/28/2019   Su Ye         Original Development

NOTES:
******************************************************************************/
int partition_buf (int arr[], short int **buf, short int *fmask_buf, int left, int right)
{
    int i = left, j = right;
    int tmp;
    short int temp_buf;
    short int temp_fmaskbuf;
    int pivot = arr[(left + right) / 2];
    int k;

    while (i <= j)
    {
        while (arr[i] < pivot)
    {
            i++;
    }
        while (arr[j] > pivot)
    {
            j--;
    }
        if (i <= j)
        {
            tmp = arr[i];


            arr[i] = arr[j];

            arr[j] = tmp;

            for (k = 0; k< TOTAL_IMAGE_BANDS; k++)
            {
                temp_buf = buf[k][i];
                buf[k][i] = buf[k][j];
                buf[k][j] = temp_buf;
            }

            temp_fmaskbuf = fmask_buf[i];
            fmask_buf[i] = fmask_buf[j];
            fmask_buf[j] = temp_fmaskbuf;


            i++;
            j--;
        }
    }

    return i;
}

/******************************************************************************
MODULE:  partition_index

PURPOSE:  partition values and index used for the quick_sort routine

RETURN VALUE:
Type = int
Value           Description
-----           -----------
i               partitioned value

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
10/03/2020   Su Ye         Original Development

NOTES:
******************************************************************************/
int partition_index (int arr[],  int *index, int left, int right)
{
    int i = left, j = right;
    int tmp;
    int temp_index;
    int pivot = arr[(left + right) / 2];
    int k;

    while (i <= j)
    {
        while (arr[i] < pivot)
        {
            i++;
        }
        while (arr[j] > pivot)
        {
            j--;
        }
        if (i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            temp_index = index[i];
            index[i] = index[j];
            index[j] = temp_index;


            i++;
            j--;
        }
    }

    return i;
}

int partition_buf_sensor (int arr[], short int **buf, short int *fmask_buf, short int *sensor, int left, int right)
{
    int i = left, j = right;
    int tmp;
    short int temp_buf;
    short int temp_fmaskbuf;
    short int temp_sensor;
    int pivot = arr[(left + right) / 2];
    int k;

    while (i <= j)
    {
        while (arr[i] < pivot)
    {
            i++;
    }
        while (arr[j] > pivot)
    {
            j--;
    }
        if (i <= j)
        {
            tmp = arr[i];


            arr[i] = arr[j];

            arr[j] = tmp;

            for (k = 0; k< TOTAL_IMAGE_BANDS; k++)
            {
                temp_buf = buf[k][i];
                buf[k][i] = buf[k][j];
                buf[k][j] = temp_buf;
            }

            temp_fmaskbuf = fmask_buf[i];
            fmask_buf[i] = fmask_buf[j];
            fmask_buf[j] = temp_fmaskbuf;

            temp_sensor = sensor[i];
            sensor[i] = sensor[j];
            sensor[j] = temp_sensor;

            i++;
            j--;
        }
    }

    return i;
}
/******************************************************************************
MODULE:  partition_float

PURPOSE:  partition the sorted list

RETURN VALUE:
Type = int
Value           Description
-----           -----------
i               partitioned value

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
05/27/2019   Su Ye        Original Development

NOTES:
******************************************************************************/
int partition_float (float arr[], int left, int right)
{
    int i = left, j = right;
    float tmp;
    float pivot = arr[(left + right) / 2];

    while (i <= j)
    {
        while (arr[i] < pivot)
    {
            i++;
    }
        while (arr[j] > pivot)
    {
            j--;
    }
        if (i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    return i;
}

/******************************************************************************
MODULE:  partition_double

PURPOSE:  partition the sorted list

RETURN VALUE:
Type = int
Value           Description
-----           -----------
i               partitioned value

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
09/22/2019   Su Ye            Original Development

NOTES:
******************************************************************************/
int partition_double (double arr[], int left, int right)
{
    int i = left, j = right;
    double tmp;
    double pivot = arr[(left + right) / 2];

    while (i <= j)
    {
        while (arr[i] < pivot)
    {
            i++;
    }
        while (arr[j] > pivot)
    {
            j--;
    }
        if (i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    return i;
}


/******************************************************************************
MODULE:  get_variables
PURPOSE:  Gets the command-line arguments and validates that the required
arguments were specified.
RETURN VALUE:
Type = int
Value           Description
-----           -----------
FAILURE         Error getting the command-line arguments or a command-line
                argument and associated value were not specified
SUCCESS         No errors encountered
HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
11/15/2018  Su Ye            orginal develop
******************************************************************************/
int get_variables
(
    int argc,              /* I: number of cmd-line args                    */
    char *argv[],          /* I: string of cmd-line args                    */
    int *mode,              /* O: 1 - pixel-based; 2 - scanline-based;       */
                           /*    3 - wholescene                             */
    char *in_path,         /* O: directory locaiton for input data          */
    char *out_path,        /* O: directory location for output files        */
    int *n_cores,            /* O: number of cores                            */
    int *row,              /* O: (required for mode 1 and mode 2)           */
                           /*    row number for the pixel                   */
    int *col,              /* O: (required for mode 1)                      */
                           /* O: col number for the pixel                   */
    int *method,
    char *user_mask_path,   /* O: directory location for user_mask           */
    double *probability_threshold,
    int *min_days_conse,
    bool *b_fastmode,
    bool *b_outputCSV,
    int *training_type, /* for training process*/
    int *monitorwindow_lowerlim, /* for training process*/
    int *monitorwindow_upperlim, /* for training process*/
    int *bandselection_bit,
    char *classification_config
)
{
    char cwd[MAX_STR_LEN]; // current directory path
    char var_path[MAX_STR_LEN];
    FILE *var_fp;
    char line1[MAX_STR_LEN], line2[MAX_STR_LEN], line3[MAX_STR_LEN],
            line4[MAX_STR_LEN], line5[MAX_STR_LEN],
            line6[MAX_STR_LEN], line7[MAX_STR_LEN], line8[MAX_STR_LEN],
            line9[MAX_STR_LEN], line10[MAX_STR_LEN], line11[MAX_STR_LEN],
            line12[MAX_STR_LEN], line13[MAX_STR_LEN], line14[MAX_STR_LEN],
            line15[MAX_STR_LEN], line16[MAX_STR_LEN];
    int output_mode;
    char FUNC_NAME[] = "get_variables";

    // when there is no variable command-line argument,
    // use the default variable text path
    if(argc < 2)
    {
        getcwd(cwd, sizeof(cwd));
        //printf("getvariable");
        sprintf(var_path, "%s/%s", cwd, "variables"); //HLS
    }
    else if(argc == 2)
    {
        strcpy (var_path, argv[1]);
    }
    else
    {
        RETURN_ERROR("only the argument for variable file "
                     "path can be added in the command line", FUNC_NAME, ERROR);
    }

    var_fp = fopen(var_path, "r");

    if(var_fp == NULL)
    {
        RETURN_ERROR("There is no variable file in the bin folder", FUNC_NAME, ERROR);
    }


    fscanf(var_fp, "%s\n", line1);
    *mode = atoi(strchr(line1, '=') + 1);


    fscanf(var_fp, "%s\n", line2);
    //in_path = strchr(line2, '=') + 1;getvariable
    strcpy(in_path, strchr(line2, '=') + 1);



    fscanf(var_fp, "%s\n", line3);
    strcpy(out_path, strchr(line3, '=') + 1);

    fscanf(var_fp, "%s\n", line4);
    *n_cores = atoi(strchr(line4, '=') + 1);


    fscanf(var_fp, "%s\n", line5);
    strcpy(user_mask_path, strchr(line5, '=') + 1);


    fscanf(var_fp, "%s\n", line6);
    *row = atoi(strchr(line6, '=') + 1);

    fscanf(var_fp, "%s\n", line7);
    *col = atoi(strchr(line7, '=') + 1);

    fscanf(var_fp, "%s\n", line8);
    *method = atoi(strchr(line8, '=') + 1);

    fscanf(var_fp, "%s\n", line9);
    *probability_threshold = atof(strchr(line9, '=') + 1);

    fscanf(var_fp, "%s\n", line10);
    *min_days_conse = atoi(strchr(line10, '=') + 1);

    fscanf(var_fp, "%s\n", line11);
    output_mode = atoi(strchr(line11, '=') + 1);

    // second digit
    if(output_mode % 10 == 1)
        *b_fastmode = FALSE;
    else
        *b_fastmode = TRUE;

    // first digit
    if(output_mode / 10 == 1)
        *b_outputCSV = TRUE;
    else
        *b_outputCSV = FALSE;

    fscanf(var_fp, "%s\n", line12);
    *training_type = atoi(strchr(line12, '=') + 1);
//    if (atoi(strchr(line12, '=') + 1)==0)
//        *training_type = FALSE;
//    else
//        *training_type = TRUE;

    fscanf(var_fp, "%s\n", line13);
    *monitorwindow_lowerlim = atoi(strchr(line13, '=') + 1);

    fscanf(var_fp, "%s\n", line14);
    *monitorwindow_upperlim = atoi(strchr(line14, '=') + 1);

    fscanf(var_fp, "%s\n", line15);
    *bandselection_bit = atoi(strchr(line15, '=') + 1);

    fscanf(var_fp, "%s\n", line16);
    strcpy(classification_config, strchr(line16, '=') + 1);
//    *format = atoi(strchr(line16, '=') + 1);

    // check if variable inputs are complete
    if (*mode == 0)
    {
        RETURN_ERROR("Variables error: please define 'mode' in "
                     "the variable file: 1 - pixel-based; 2 - scanline-based;"
                     " 3 - whole scene ", FUNC_NAME, ERROR);

    }
    if (*in_path == '\0')
    {
        RETURN_ERROR("Variables error: 'in_path' "
                     "cannot be empty in 'Variables' file!", FUNC_NAME, ERROR);

    }
    if (*out_path == '\0')
    {
        RETURN_ERROR("Variables error: 'out_path' "
                     "cannot be empty in 'Variables' file!", FUNC_NAME, ERROR);
    }
    if (*mode == 3)
    {
        if(*n_cores == 0)
        {
            RETURN_ERROR("Variables error: 'n_cores' "
                         "cannot be empty in 'Variables' "
                         "file for mode 3!", FUNC_NAME, ERROR);

        }

    }
    if ((*mode == 1)||(*mode == 2))
    {
        if(*row == 0)
        {
            RETURN_ERROR("Variables error: 'row' cannot "
                         "be empty in 'Variables' file "
                         "for mode 2 or 3!", FUNC_NAME, ERROR);

        }
    }

    if (*mode == 1)
    {
        if(*col == 0)
        {
            RETURN_ERROR("Variables error: 'col' "
                         "cannot be empty in 'Variables' "
                         "file for mode 1!", FUNC_NAME, ERROR);
        }
    }

//    fscanf(var_fp, "%s\n", line);
//    fscanf(var_fp, "%s\n", line);
//    fscanf(var_fp, "%s\n", line);
//    fscanf(var_fp, "in_path=%s\n", in_path);
//    fscanf(var_fp, "out_path=%s\n", out_path);
//    fscanf(var_fp, "n_cores=%d\n", n_cores);
//    fscanf(var_fp, "user_mask_path=%s\n", user_mask_path);
//    fscanf(var_fp, "row=%d\n", row);
//    fscanf(var_fp, "col=%d\n", col);

    fclose(var_fp);
    return SUCCESS;

}

/******************************************************************************
MODULE:  get_classificationconfig
PURPOSE:  Gets the command-line arguments and validates that the required
arguments were specified.
RETURN VALUE:
Type = int
Value           Description
-----           -----------
FAILURE         Error getting the command-line arguments or a command-line
                argument and associated value were not specified
SUCCESS         No errors encountered
HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
04/12/2018  Su Ye            orginal develop
******************************************************************************/
int get_classificationconfig
(
    char* var_path,
    char *xgboost_model_path,
    int *specific_label,
    char *auxiliary_var1
)
{
    FILE *var_fp;
    char line1[MAX_STR_LEN], line2[MAX_STR_LEN], line3[MAX_STR_LEN];
    int output_mode;
    char FUNC_NAME[] = "get_classificationconfig";

    var_fp = fopen(var_path, "r");

    if(var_fp == NULL)
    {
        RETURN_ERROR("There is no variable file in the bin folder", FUNC_NAME, ERROR);
    }

    fscanf(var_fp, "%s\n", line1);
    strcpy(xgboost_model_path, strchr(line1, '=') + 1);

    fscanf(var_fp, "%s\n", line2);
    *specific_label = atoi(strchr(line2, '=') + 1);

    fscanf(var_fp, "%s\n", line3);
    strcpy(auxiliary_var1, strchr(line3, '=') + 1);

    fclose(var_fp);
    return SUCCESS;

}


/******************************************************************************
MODULE:  get_args

PURPOSE:  Gets the command-line arguments and validates that the required
arguments were specified.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
FAILURE         Error getting the command-line arguments or a command-line
                argument and associated value were not specified
SUCCESS         No errors encountered

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
1/5/2015    Song Guo         Original Development
20151203    Brian Davis      Added arguments for input and output
                             directories, and scene list file.
20191124    Su Ye            Modified for sccd

NOTES:
  1. Memory is allocated for the input and output files.  All of these should
     be character pointers set to NULL on input.  The caller is responsible
     for freeing the allocated memory upon successful return.
  2. chi2inv(T_cg, num_bands) = chi2inv(0.99, 5) = 15.0863
  3. chi2inv(T_max_cg, num_bands) = chi2inv(1-1e-6, 5) = 35.8882
******************************************************************************/

int get_args
(
    int argc,              /* I: number of cmd-line args                    */
    char *argv[],          /* I: string of cmd-line args                    */
    int *mode,             /* O: 1 - pixel-based; 2 - scanline-based; 3 - wholescene; 4-validation */
    char *in_path,         /* O: directory locaiton for input data          */
    char *out_path,        /* O: directory location for output files        */
    int *n_cores,          /* O: the number of cores                        */
    int *row,              /* O: row number for the pixel                   */
    int *col,              /* O: col number for the pixel                   */
    int *method,
    char *user_mask_path,
    double *probability_threshold,
    int *min_days_conse,
    bool *b_fastmode,
    bool *b_outputCSV
)
{
    int c;                         /* current argument index                */
    int output_mode;
    int option_index;              /* index for the command-line option     */
    static int verbose_flag = 0;   /* verbose flag                          */
    char errmsg[MAX_STR_LEN];      /* error message                         */
    char FUNC_NAME[] = "get_args"; /* function name                         */
    static struct option long_options[] = {
        {"mode", required_argument, 0, 'm'},
        {"in-path", required_argument, 0, 'i'},
        {"out-path", required_argument, 0, 'o'},
        {"n_cores", required_argument, 0, 'n'},
        {"row", required_argument, 0, 'r'},
        {"col", required_argument, 0, 'c'},
        {"method", required_argument, 0, 'me'},
        {"user_mask_path", required_argument, 0, 'u'},
        {"probability_threshold", required_argument, 0, 'p'},
        {"min_days_conse", required_argument, 0, 'mi'},
        {"output_mode", required_argument, 0, 'ou'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    /******************************************************************/
    /*                                                                */
    /* Loop through all the cmd-line options                          */
    /*                                                                */
    /******************************************************************/

    while (1)
    {
        /* optstring in call to getopt_long is empty since we will only
           support the long options */
        c = getopt_long (argc, argv, "", long_options, &option_index);
        if (c == -1)
        {

            /**********************************************************/
            /*                                                        */
            /* Out of cmd-line options                                */
            /*                                                        */
            /**********************************************************/

            break;
        }

        switch (c)
        {
            case 0:

                /******************************************************/
                /*                                                    */
                /* If this option set a flag, do nothing else now.    */
                /*                                                    */
                /******************************************************/

                if (long_options[option_index].flag != 0)
                {
                    break;
                }
                sprintf (errmsg, "option %s\n", long_options[option_index].name);
                if (optarg)
                {
                    sprintf (errmsg, "option %s with arg %s\n",
                             long_options[option_index].name, optarg);
                }
                RETURN_ERROR (errmsg, FUNC_NAME, ERROR);
                break;
            case 'm':              /* mode */
                *mode = atoi (optarg);
                break;
            case 'i':
                strcpy (in_path, optarg);
                break;
            case 'o':
                strcpy (out_path, optarg);
                break;
            case 'n':
                *n_cores = atoi (optarg);
                break;
            case 'r':
                *row = atoi (optarg);
                break;
            case 'c':
                *col = atoi (optarg);
                break;
            case 'me':
                *method = atoi (optarg);
                break;
            case 'u':
                strcpy (user_mask_path, optarg);
            case 'p':
                *probability_threshold = atof (optarg);
                break;
            case 'mi':
                *min_days_conse = atoi (optarg);
                break;
            case 'ou':
                output_mode = atoi (optarg);
                if(output_mode % 10 == 1)
                    *b_fastmode = TRUE;
                else
                    *b_fastmode = FALSE;

                if(output_mode / 10 == 1)
                    *b_outputCSV = TRUE;
                else
                    *b_outputCSV = FALSE;
                break;
            case 'h':              /* help */
                usage_message ();
                exit (SUCCESS);
                break;
            case '?':
            default:
                sprintf (errmsg, "Unknown option %s", argv[optind - 1]);
                usage_message ();
                RETURN_ERROR (errmsg, FUNC_NAME, ERROR);
                break;
        }
    }

    /******************************************************************/
    /*                                                                */
    /* Check the input values                                         */
    /*                                                                */
    /******************************************************************/

    if (*row < 0)
    {
        sprintf (errmsg, "row number must be > 0");
        RETURN_ERROR(errmsg, FUNC_NAME, FAILURE);
    }

    if (*col < 0)
    {
        sprintf (errmsg, "column number must be > 0");
        RETURN_ERROR(errmsg, FUNC_NAME, FAILURE);
    }

    /******************************************************************/
    /*                                                                */
    /* If in_path and out_path were not specified, assign local       */
    /* directory, so that pre-pending a directory/path later on will  */
    /* not cause an error, but instead result in ./<filename> .       */
    /*                                                                */
    /******************************************************************/

    if (strlen(in_path) == 0)
    {
        strcpy (in_path, ".");
    }
    if (strlen(out_path) == 0)
    {
        strcpy (out_path, ".");
    }


    return (SUCCESS);
}

/******************************************************************************
MODULE:  usage_message

PURPOSE:  Prints the usage information for this application.

RETURN VALUE:
Type = None

HISTORY:
Date        Programmer       Reason
--------    ---------------  -------------------------------------
2/15/2015   Song Guo         Original Development
20160304    Brian Davis      Updated to reflect stdin and stdout.

******************************************************************************/
int usage_message ()
{
    printf ("\n");
    printf ("Continuous Change Detection and Classification\n");
    printf ("Version 05.01\n");
    printf ("\n");
    printf ("usage:\n");
    printf ("ccdc"
            " --row=<input row number>"
            " --col=<input col number>"
            " [--in-path=<input directory>"
            " [--out-path=<output directory[>"
            " [--data-type=<tifs|bip[>"
            " [--scene-list-file=<file with list of sceneIDs>]"
            " [--verbose]\n");

    printf ("\n");
    printf ("where the following parameters are required:\n");
    printf ("    --row=: input row number\n");
    printf ("    --col=: input col number\n");
    printf ("\n");
    printf ("and the following parameters are optional:\n");
    printf ("    --in-path=: input data directory location\n");
    printf ("    --out-path=: directory location for output files\n");
    printf ("    --data-type=: type of input data files to ingest\n");
    printf ("    --scene-list-file=: file name containing list of sceneIDs"
            " (default is all files in in-path)\n");
    printf ("    -verbose: should intermediate messages be printed?"
            " (default is false)\n");
    printf ("\n");
    printf ("ccdc --help will print the usage statement\n");
    printf ("\n\n");
    printf ("Example:\n");
    printf ("ccdc"
            " --row=3845"
            " --col=2918"
            " --in-path=/data/user/in"
            " --out-path=/home/user/out"
            " --data-type=bip"
            " --scene-list-file=/home/user/scene_list.txt"
            " --verbose\n\n");
    printf ("An example of how to pipe input from stdin and output to stdout:\n");
    printf ("ccdc"
            " --row=3845"
            " --col=2918"
            " --in-path=stdin"
            " --out-path=stdout"
            " --verbose < pixel_value_text_file.txt > coeffs_results_text_file.txt\n\n");
    printf ("The stdout option eliminates the creation of the output binary file, \n");
    printf ("coeffs are just printed to stdout.  It could be "
            "re-directed to a text file, or piped to another program.\n");
    printf ("\nNote: Previously, the ccdc had to be run from the directory"
            " where the input data are located.\n");
    printf ("      Now, input and output directory locations specifications are used.\n");
    printf ("      If in-path or out-path are not specified, current working directory is assumed.\n");
    printf ("      If scene-file-name is not specified, all scenes in in-path are processed.\n\n");
    return (SUCCESS);
}

