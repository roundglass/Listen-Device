from distutils.log import error


class DataChecking():
    def __init__(self):
        super().__init__()
        

    def CheckData(self,RecordMode,Record,Sleep,Periods):
        if (RecordMode == 'Interval Mode'):
            if(Record and Sleep):
                Record = int(Record)
                Sleep = int(Sleep)
                if(3600 < Record  or 3600 < Sleep):
                    return 104
            elif (Record and not Sleep):
                return 101
            elif(Sleep and not Record):
                return 102
            else:
                return 103            

            if(Periods):
                for Period in Periods:  
                    Period = Period.split(" - ")

                    A = int(Period[0])
                    B = int(Period[1])
       
                    if (A >2400 or B >2400):
                        return 105
                    elif(A == B):
                        return 106
        elif(RecordMode == 'Continuous Mode'):
            if(Record):
                Record = int(float(Record))
                if(Record >3600):
                    return 104
            else:
                return 102




                    

            


        
        



        
        





