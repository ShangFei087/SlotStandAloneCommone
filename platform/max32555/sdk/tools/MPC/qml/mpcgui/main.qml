// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1



Rectangle {
    id:mpcGUI
    width: 800
    height: 600
    color: "Lightgray"



    //#############
    //## SIGNALS ##
    //#############################################################################################
    signal startProductionProcess(int index);
    signal finishProductionProcess(int index);


    //#######################
    //## FUNCTIONS - SLOTS ##
    //#############################################################################################


        /*function productionItemDataChanged(index,propertyName,propertyValue)
        {

            switch(propertyName)
            {
            case "state":
                console.log("Process id("+index+") is "+propertyValue+" !!!!!");
                mpcRowElements.changeItemState(index,propertyValue);
                break;

            default:
                break;
            }
        }*/




    //################
    //## COMPONENTS ##
    //#############################################################################################


        MXIMMPCHeader {
            id:mpcProdArrayHeader
            x:0
            y:5

        }

        MXIMMPCRow {

           id:mpcRowElements
           x:0
           y:mpcProdArrayHeader.globalHeight+mpcProdArrayHeader.y

           width: parent.width
           height:parent.height-mpcProdArrayHeader.globalHeight

           mpcModel:mximProduction


        }


}

