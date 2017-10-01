static int classify( const int *features )
{
   if ( features[0] == 0x0 /* A */ )
   {
      if ( features[1] == 0x2 /* C */ )
      {
         return 1;
      }
      else
      {
         if ( features[1] == 0x13 /* T */ )
         {
            if ( features[2] == 0x0 /* A */ )
            {
               return 1;
            }
            else
            {
               return 0;
            }
         }
         else
         {
            return 0;
         }
      }
   }
   else
   {
      if ( features[0] == 0x4 /* E */ )
      {
         if ( features[1] == 0x13 /* T */ )
         {
            if ( features[2] == 0xd /* N */ )
            {
               return 1;
            }
            else
            {
               return 0;
            }
         }
         else
         {
            return 0;
         }
      }
      else
      {
         if ( features[0] == 0x8 /* I */ )
         {
            if ( features[1] == 0x4 /* E */ )
            {
               return 1;
            }
            else
            {
               if ( features[1] == 0xd /* N */ )
               {
                  if ( features[2] == 0x0 /* A */ )
                  {
                     if ( features[3] == 0x18 /* Y */ )
                     {
                        return 1;
                     }
                     else
                     {
                        return 0;
                     }
                  }
                  else
                  {
                     return 0;
                  }
               }
               else
               {
                  return 0;
               }
            }
         }
         else
         {
            if ( features[0] == 0xd /* N */ )
            {
               return 1;
            }
            else
            {
               if ( features[0] == 0xe /* O */ )
               {
                  return 1;
               }
               else
               {
                  if ( features[0] == 0x11 /* R */ )
                  {
                     return 1;
                  }
                  else
                  {
                     if ( features[0] == 0x12 /* S */ )
                     {
                        return 1;
                     }
                     else
                     {
                        if ( features[0] == 0x19 /* Z */ )
                        {
                           return 1;
                        }
                        else
                        {
                           if ( features[0] == 0x1d /* Ã */ )
                           {
                              return 1;
                           }
                           else
                           {
                              return 0;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
}


static int mapCharacter( char c )
{
	int value = (unsigned char) c;
	if ( value >= 0x41 && value <= 0x5A )
		return value - 0x41;
	if ( value >= 0x61 && value <= 0x7A )
		return value - 0x61;
	if ( value >= 0xC0 && value <= 0xDF )
		return value - 0xC0 + 26;
	if ( value >= 0xE0 && value <= 0xFF )
		return value - 0xE0 + 26;
	return -1;
}


int classifyWord( const char *word, int len )
{
	int features[4];
	int i = 0;
	const char *ptr;
	if (word == 0 || len < 4) return -1;
	ptr = word + (len - 1);
	for (; i < 4; ++i, --ptr)
	{
		if (i < len)
			features[i] = mapCharacter(*ptr);
		else
			features[i] = -1;
	}
	return classify(features);
}
