import React, {useState} from 'react';
import {AgGridReact} from 'ag-grid-react';
import { withRouter } from 'react-router-dom';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-alpine.css';


function Transactions(props)  {

    // set to default data
    const [rowData, setRowData] = useState();

    const columnDefs = [
        { field: 'id' },
    ];

    React.useEffect(() => {
        // fetch('https://api2.binance.com/api/v3/ticker/24hr')
        fetch('http://127.0.0.1:8001/nodetransactions')
            .then(result => result.json())
            .then(rowData => { 
                //console.log(rowData);
                let newData = JSON.stringify(rowData);
                setRowData(JSON.parse(newData));
            })
    }, []);

   return (
       <div className="ag-theme-alpine" style={{height: 355}}>   
           <AgGridReact
                defaultColDef={{sortable: true, filter: true }}
                pagination={true}
                rowData={rowData}
                columnDefs={columnDefs}
                onCellClicked={(params) => console.log(params.data)}>
           </AgGridReact>
       </div>
       
   )

};

export default withRouter(Transactions);