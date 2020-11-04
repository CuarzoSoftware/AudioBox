#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#include <AudioBox.h>

struct stat st = {};

// Usage ./Bundler dsp_path destination_path
int main(int argc,char *argv[])
{
    // Only AudioBox DSP file
    if(argc == 3)
    {
        // Check if file exists
        if(access(argv[1], F_OK) != -1)
        {
            // Check if destination path exists
            if (stat(argv[3], &st) == -1)
            {
                char *from = (char*)malloc(strlen(argv[1]) + 64);
                strcpy(from,argv[1]);
                if(from[strlen(from)-1] != '/')
                    from = strcat(from,"/");

                from = strcat(from,"Core.dsp");

                // Loads the AudioBox
                AudioBox *box = loadDSP(from);

                // Gets the name
                const char *name = box->getName();
                printf("Plugin Name:%s\n",name);

                // Creates the plugin bundle
                char *destination = (char*)malloc(sizeof(char)*(strlen(name) + strlen(argv[2]) + 64));
                strcpy(destination,argv[2]);
                if(destination[strlen(destination)-1] != '/')
                    destination = strcat(destination,"/");
                destination = strcat(destination,name);
                destination = strcat(destination,".audiobox");
                mkdir(destination, 0700);

                // Creates the DSP sub dir
                char *dspDir = strcat(destination,"/DSP");
                mkdir(dspDir, 0700);

                char *destDSP = strcat(dspDir,"/Core");

                rename(from,destDSP);

                if(box->getUIType() != AudioBox::UI::No)
                {
                    printf("UI: Yes\n");
                    char *uiDir = strcat(destination,"/UI");
                    mkdir(uiDir, 0700);
                }
                else
                {
                    printf("UI: No\n");
                }

            }
            else
            {
                printf("Destination directory doesn't exist.\n");
                return 1;
            }
        }
        else
        {
            printf("DSP file doesn't exist.\n");
            return 1;
        }
    }
    else
    {
        printf("Missing arguments.\nUSAGE:\n./Bundler dsp_path destination_path");
        return 1;
    }

    return 0;
}
