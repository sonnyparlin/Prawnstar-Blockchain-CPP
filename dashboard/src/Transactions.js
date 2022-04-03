import React from 'react';

class Transactions extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { transactions: [] };
    }

    componentDidMount() {
        this.getWalletAmount();
    }

    getWalletAmount() {

        fetch("http://127.0.0.1:8001/nodetransactions")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                this.state.transactions.push( jsonData );
                this.state.transactions = this.state.transactions[0]
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    render() { 
        return (
            <pre>
                <small>
                    <ul>
                        {this.state.transactions.map(item => <li key={item}>{item}</li>)}
                    </ul>
                </small>
            </pre>
        );
    }
}

export default Transactions;