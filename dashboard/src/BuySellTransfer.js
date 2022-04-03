import React from 'react';

function BuySellTransfer() {

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
        <table>
        <tbody>
        <tr>
        <td>
        <button className="btn btn-primary" onClick={handleClick}>
            Buy
        </button>
        </td>
        <td>
        <button className="btn btn-primary" onClick={handleClick}>
            Sell
        </button>
        </td>
        <td>
        <button className="btn btn-primary" onClick={handleClick}>
            Transfer
        </button>
        </td>
        <td>
        <button className="btn btn-primary" onClick={handleClick}>
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