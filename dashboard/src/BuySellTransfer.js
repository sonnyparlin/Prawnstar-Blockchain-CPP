import React, { useState } from 'react';

function BuySellTransfer() {

    const [hidden, setHidden] = useState(false);

    var jsonData = {
      "users": [
          {
              "name": "alan", 
              "age": 23,
              "username": "aturing"
          },
          {
              "name": "john", 
              "age": 29,
              "username": "__john__"
          }
      ]
    }
  
    function handleClick() {
      
      // Send data to the backend via POST
      fetch('http://------------:8080/', {  // Enter your IP address here
        method: 'POST', 
        mode: 'cors', 
        body: JSON.stringify(jsonData) // body data type must match "Content-Type" header
  
      })
      
    }
  
    return (
    <div>
        <div className="transactionSpace">
        
            <div className="buyForm" hidden={!hidden}>
                <div className="fleft">
                <input type="text" className="form-control-plaintext" id="amount" placeholder="amount" />
                </div>
            <button type="submit" className="btn btn-primary mb-2">Buy</button>
            </div>

        </div>
        <table>
        <tbody>
        <tr>
        <td>
        <button className="btn btn-primary" onClick={() => setHidden(!hidden)}>
            Buy
        </button>
        </td>
        <td>
        <button className="btn btn-primary" onClick={() => setHidden(!hidden)}>
            Sell
        </button>
        </td>
        <td>
        <button className="btn btn-primary" onClick={() => setHidden(!hidden)}>
            Transfer
        </button>
        </td>
        <td width="240px" align="right">
        <button className="btn btn-primary" onClick={() => setHidden(!hidden)}>
            Stake
        </button>
        </td>
        </tr>
        </tbody>
        </table>
    </div>
    );
  
  }
  
  export default BuySellTransfer;