import React from 'react';

class Wallet extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { amount: "", address: "" };
    }

    componentDidMount() {
        this.getWalletAmount();
    }

    getWalletAmount() {

        fetch("http://127.0.0.1:8001/nodewallet")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                this.setState({ amount: jsonData.amount });
                this.setState({ address: jsonData.address });
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    render() { 
        return (
           
            <pre>
                <small>Balance: {this.state.amount} Prawn</small><br/>
                <small>Address: {this.state.address}</small>
            </pre>
      
        );
    }
}

export default Wallet;
