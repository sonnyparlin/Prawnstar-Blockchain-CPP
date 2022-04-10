import React, {useState, useEffect} from 'react';
import {AgGridReact} from 'ag-grid-react';
import { withRouter } from 'react-router-dom';
import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-alpine.css';


function Transactions(props)  {

    // set to default data
    const [rowData, setRowData] = useState();

    const [amount, setAmount] = useState([]);
    const [id, setId] = useState("");
    const [receiverAddress, setReceiverAddress] = useState("");
    const [senderAddress, setSenderAddress] = useState("");
    // const [senderPublicKey, setSenderPublicKey] = useState("");
    // const [signature, setSignature] = useState("");
    const [timestamp, setTimestamp] = useState("");
    const [type, setType] = useState("");

    const columnDefs = [
        { field: 'id' },
    ];

    useEffect(() => {
        // fetch('https://api2.binance.com/api/v3/ticker/24hr')
        fetch('http://127.0.0.1:8001/nodetransactions')
            .then(result => result.json())
            .then(rowData => { 
                //console.log(rowData);
                let newData = JSON.stringify(rowData);
                setRowData(JSON.parse(newData));
            })
    }, []);

    const getTransaction = (txid) => {

        fetch("http://127.0.0.1:8001/tx/"+txid)
            .then(response => response.json())
            .then((jsonData) => {
                setAmount(jsonData.amount);
                setId(jsonData.id);
                setReceiverAddress(jsonData.receiverAddress);
                setSenderAddress(jsonData.senderAddress);
                // setSenderPublicKey(jsonData.senderPublicKey);
                // setSignature(jsonData.signature);
                setTimestamp(jsonData.timestamp);
                setType(jsonData.type);
                document.getElementsByClassName('ag-theme-alpine')[0].style.display = 'none';
                document.getElementsByClassName('txDetail')[0].style.display = 'block';
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    return (
        <>
            <div className="txDetail" style={{display: "None"}}>
                <small>
                    <div id={id}>
                        <div className="fright">
                            <button type="submit" className="btn btn-primary mb-2" onClick={() => 
                                {
                                    document.getElementsByClassName('ag-theme-alpine')[0].style.display = 'block';
                                    document.getElementsByClassName('txDetail')[0].style.display = 'none';
                                }
                            }>Close</button>
                        </div>
                        <br/><br/>
                        <span><b>id:</b></span><span className="fright">{id}</span>
                        <div></div>
                        <span><b>amount:</b></span><span className="fright">${amount}</span>
                        <div></div>
                        <span><b>from:</b></span><span className="fright">{senderAddress}</span>
                        <div></div>
                        <div></div>    
                        <span><b>to:</b></span><span className="fright">{receiverAddress}</span>
                        <div></div> 
                        <span><b>time:</b></span><span className="fright">{timestamp}</span>
                        <div></div>   
                        <span><b>type:</b></span><span className="fright">{type}</span>
                    </div>
                </small>
            </div>

            <div className="ag-theme-alpine" style={{height: 355}}>   
            <AgGridReact
                    defaultColDef={{sortable: true, filter: true }}
                    pagination={true}
                    rowData={rowData}
                    columnDefs={columnDefs}
                    onCellClicked={(params) => getTransaction(params.data.id)}>
            </AgGridReact>
            </div>
        </>
   );

};

export default withRouter(Transactions);