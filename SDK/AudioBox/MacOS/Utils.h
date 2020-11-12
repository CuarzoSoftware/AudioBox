#ifdef __APPLE__
#ifndef UTILSMACOS_H
#define UTILSMACOS_H

    // Converts CFString to char array
    char * CFStringToChar(CFStringRef aString)
    {
        if (aString == NULL)
            return NULL;

        CFIndex length = CFStringGetLength(aString);
        CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
        char *buffer = (char *)malloc(maxSize);

        if (CFStringGetCString(aString, buffer, maxSize,kCFStringEncodingUTF8))
            return buffer;

        free(buffer);
        return NULL;
    }

#endif
#endif
