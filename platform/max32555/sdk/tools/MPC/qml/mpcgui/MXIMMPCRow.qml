// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {

    id: mpcProdArrayRow

    //################
    //## PROPERTIES ##
    //#############################################################################################

        property variant    mpcModel

        property int        nbProdSubColumn:5

        property int        colSpace:5
        property int        rowSpace:5

        property int        globalHeight:50

        property int        commDeviceWidth:150


        property int        prodColWidth:(parent.width-commDeviceWidth)-(2*rowSpace)
        property int        prodSubColWidth:(prodColWidth-((nbProdSubColumn-1)*rowSpace))/nbProdSubColumn

        property color      cellColor:"black"

        property string     undef:"undefined";

    //###############
    //## FUNCTIONS ##
    //#############################################################################################

        function getCellResltColor(result)
        {
            switch(result)
            {
            case "OK":
                return "green";

            case "NOK":
                return "red";

            default:
                return "red";

            }
        }

    function getProdPhase(phase)
    {
        var str1 = new String( "5" );
        var str2 = new String( phase );
        var str3 = new String( "undefined" );

        var index = str1.localeCompare( str2 );
        console.log("localeCompare first :" + index );


        if(index === 0)
        {
            //console.log("QML - mpcProdPhaseValue - text= \"undefined\ (phase === 5)");
            //console.log("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
            //var value="undefined";
            //return value;
            console.log("Phase value:"+str3);

            return str3;
        }
        else
        {
            //console.log("QML - mpcProdPhaseValue - text= "+phase);
            //console.log("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
            console.log("Phase value:"+str2);
            return str2;
        }
    }


    //---------------------------------------------------------------------------------------------

    //#############
    //## SIGNALS ##
    //#############################################################################################




    //---------------------------------------------------------------------------------------------



    Component {
        id: mpcProdDelegate



        Item {

            id:mpcProdDelegateItem
            objectName: "mpcProdDelegateItem"

            width:parent.width
            height: globalHeight

            state:prodState


            Row {

                id:mpcProdDelegateItemRow

                objectName: "mpcProdDelegateItemRow"

                spacing:colSpace

                //------------------------------------
                //-- Communication device name
                //------------------------------------
                Rectangle {
                    id:mpcCommDevice

                    width:commDeviceWidth
                    height: globalHeight


                    color: "black"
                    border.width: 2
                    border.color: "white"



                    Text{
                        id:mpcCommDeviceName

                        width: parent.width
                        height: parent.height

                        text:commDeviceName

                        color:"white"

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter


                    }
                }

                //------------------------------------
                //-- Production process start/finish
                //------------------------------------
                Rectangle {
                    id:mpcProdProcessStarting


                    width:prodSubColWidth
                    height:globalHeight


                    color: "black"
                    border.width: 2
                    border.color: "white"


                    MXIMMPCButton
                    {
                        id:mpcProdBtnStart

                        width: parent.width
                        height: parent.height

                        textColor:"white"
                        bkColor:parent.color
                        btnText:"Start"


                        onBtnTextChanged: {
                            console.log("QML - MPCProdBtnStart btn text changed");
                        }

                        onButtonClicked: {

                            list.currentIndex=index;

                            //console.log("QML - Start production process on device (index=%1): %2",index,MPCProdArrayRow.commDeviceName);
                            //console.log("QML - State: "+MPCProdDelegateItem.state);
                            //console.log("QML - ProdState: "+ prodState);


                            switch(prodState)
                            {
                                case "stopped":
                                    startProductionProcess(index);
                                break;
                                case "finished":
                                    finishProductionProcess(index);
                                break;
                                case "started":
                                default:
                                break;


                            }


                        }

                    }



                }

                //------------------------------------
                //-- Production phase
                //------------------------------------
                Rectangle {
                    id:mpcProdPhase

                    width:prodSubColWidth
                    height:globalHeight

                    color: "black"
                    border.width: 2
                    border.color: "white"



                    Text{
                        id:mpcProdPhaseValue

                        width: parent.width
                        height: parent.height


                        text:phase/*{
                            return getProdPhase(phase);
                        }*/

                        color:"white"

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter


                    }
                }

                //------------------------------------
                //-- Production USN
                //------------------------------------
                Rectangle {
                    id:mpcProdUSN

                    width:prodSubColWidth
                    height:globalHeight

                    color: "black"
                    border.width: 2
                    border.color: "white"

                    Text{
                        id:mpcProdUSNValue

                        width: parent.width
                        height: parent.height


                        text:{
                            if(usn === "")
                            {
                                return "undefined";
                            }
                            else
                            {
                                return usn;
                            }
                        }

                        color:"white"

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter


                    }
                }

                //------------------------------------
                //-- Production process progression
                //------------------------------------
                Rectangle {
                    id:mpcProdProcessProgression

                    width:prodSubColWidth
                    height:globalHeight

                    color: "black"
                    border.width: 2
                    border.color: "white"

                    ProgressBar {
                            id: mpcProdProcessProgressBar
                            //anchors.centerIn: progressSpinner
                            width: parent.width
                            height: parent.height
                            color: "LightBlue"
                            value: progress

                            Text{
                                id:mpcProdPrcessProgressionValue

                                width: parent.width
                                height: parent.height


                                text:progress+"%"

                                color:"white"

                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter


                            }
                        }
                }

                //------------------------------------
                //-- Production process result
                //------------------------------------
                Rectangle {
                    id:mpcProdResult

                    width:prodSubColWidth
                    height:globalHeight

                    color: "black"
                    border.width: 2
                    border.color: "white"

                    Row
                    {
                        width: parent.width
                        height: parent.height
                        //spacing: colSpace
                        Text{
                            id:mpcProdResultValue

                            width: (parent.width/2)-20
                            height: parent.height


                            text:{

                                return result;
                            }

                            color:"white"

                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter//Text.left


                        }
                        Text{
                            id:mpcProdResultErrorCode

                            width: parent.width/2
                            height: parent.height


                            text:msg

                            color:"white"

                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter//Text.left


                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            list.currentIndex = index
                        }
                    }


                }




            }
            //-- End of Row element

            onStateChanged: {
                console.log("MPCProdDelegateItem - State has changed: "+mpcProdDelegateItem.state);
            }

            states: [

                State{

                    name:"stopped"

                    //-- Communication device name
                    PropertyChanges { target:mpcCommDevice;color:"black"}

                    //-- Button start/finished
                    PropertyChanges { target:mpcProdProcessStarting;color:"black"}
                    PropertyChanges { target:mpcProdBtnStart;btnText:"Start";state:'anim_start'}

                    //-- JIBE Phase/Life cycle
                    PropertyChanges { target:mpcProdPhase;color:"black"}
                    PropertyChanges { target:mpcProdPhaseValue;text:"undefined"}

                    //-- JIBE USN
                    PropertyChanges { target:mpcProdUSN;color:"black"}

                    //-- Production process (serial_sender) progression
                    PropertyChanges { target:mpcProdProcessProgression;color:"black"}

                    //-- Production process result
                    PropertyChanges { target:mpcProdResult;color:"black"}


                },

                State{

                    name:"started"

                    //-- Communication device name
                    PropertyChanges { target:mpcCommDevice;color:"blue"}

                    //-- Button start/finished
                    PropertyChanges { target:mpcProdProcessStarting;color:"blue"}
                    PropertyChanges { target:mpcProdBtnStart;btnText:"Start";state:'anim_stop'}

                    //-- JIBE Phase/Life cycle
                    PropertyChanges { target:mpcProdPhase;color:"blue"}
                    //PropertyChanges { target:mpcProdPhaseValue;text:"undefined"}

                    //-- JIBE USN
                    PropertyChanges { target:mpcProdUSN;color:"blue"}
                    //PropertyChanges { target:mpcProdUSNValue;text:"undefined"}

                    //-- Production process (serial_sender) progression
                    PropertyChanges { target:mpcProdProcessProgression;color:"blue"}

                    //-- Production process result
                    PropertyChanges { target:mpcProdResult;color:"blue"}


                },

                State{

                    name:"finished"

                    //-- Communication device name
                    PropertyChanges { target:mpcCommDevice;color:getCellResltColor(result)}

                    //-- Button start/finished
                    PropertyChanges { target:mpcProdProcessStarting;color:getCellResltColor(result)}
                    PropertyChanges { target:mpcProdBtnStart;btnText:"Finished";state:'anim_start'}

                    //-- JIBE Phase/Life cycle
                    PropertyChanges { target:mpcProdPhase;color:getCellResltColor(result)}

                    //-- JIBE USN
                    PropertyChanges { target:mpcProdUSN;color:getCellResltColor(result)}

                    //-- Production process (serial_sender) progression
                    PropertyChanges { target:mpcProdProcessProgression;color:getCellResltColor(result)}

                    //-- Production process result
                    PropertyChanges { target:mpcProdResult;color:getCellResltColor(result)}


                }


            ]
            //-- End of states property
        }

    }

    ListView {
        id: list
        width:parent.width
        height:parent.height

        //anchors.fill: parent
        model: mpcModel
        delegate: mpcProdDelegate

        focus: true

        //highlight: Rectangle { color: "lightgray"; radius: 5 }
        //highlight: mpcProdDelegateHighlighted
    }









}
